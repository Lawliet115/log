
#include <iostream>
#include <fmt/core.h>  // 引入fmt库的核心头文件
#include <fstream>

#include "sinks/effective_sink.h"
#include "compress/zstd_compress.h"
#include "crypt/aes_crypt.h"
#include "formatter/effective_formatter.h"
#include "utils/file_util.h"
#include "utils/sys_util.h"
#include "utils/timer_count.h"

namespace logger {

// 构造函数，初始化所有成员变量
EffectiveSink::EffectiveSink(Conf conf) : conf_(conf) {
  // 如果指定的路径不存在，则创建该路径
  if (!std::filesystem::exists(conf_.dir)) {
    std::filesystem::create_directories(conf_.dir);
  }
  
  // 初始化formatter_
  formatter_ = std::make_unique<EffectiveFormatter>();  

  // 创建任务运行器，tag为20010305
  task_runner_ = NEW_TASK_RUNNER(20010305);  

  // 初始化加密相关的ECDH密钥对
  auto ecdh_key = crypt::GenECDHKey();
  auto client_pri = std::get<0>(ecdh_key);
  client_pub_key_ = std::get<1>(ecdh_key);
  LOG_INFO("EffectiveSink: client pub size {}", client_pub_key_.size());
  
  // 获取服务器公钥并生成共享密钥
  // std::string svr_pub_key_bin = crypt::HexKeyToBinary(conf_.pub_key);
  // std::string shared_secret = crypt::GenECDHSharedSecret(client_pri, svr_pub_key_bin);
  std::string shared_secret = crypt::GenECDHSharedSecret(client_pri, conf_.pub_key);
  
  // 初始化AESCrypt加密器
  crypt_ = std::make_unique<crypt::AESCrypt>(shared_secret);
  
  // 初始化压缩器
  compress_ = std::make_unique<compress::ZstdCompress>();
  
  // 初始化内存映射缓存
  master_cache_ = std::make_unique<MMap>(conf_.dir / "master_cache");
  slave_cache_ = std::make_unique<MMap>(conf_.dir / "slave_cache");
  
  // 如果创建缓存失败，抛出异常
  if (!master_cache_ || !slave_cache_) {
    throw std::runtime_error("EffectiveSink::EffectiveSink: create mmap failed");
  }

  // 如果从缓存不为空，先将其内容写入文件
  if (!slave_cache_->Empty()) {
    is_slave_free_.store(true);
    PrepareToFile_();
    WAIT_TASK_IDLE(task_runner_);
  }

  // 如果主缓存不为空且从缓存为空，则交换主从缓存并写入文件
  if (!master_cache_->Empty()) {
    if (is_slave_free_) {
      is_slave_free_.store(false);
      SwapCache_();
    }
    PrepareToFile_();
  }

  // 定时执行日志文件淘汰任务
  POST_REPEATED_TASK(task_runner_, [this]() { ElimateFiles_(); }, conf_.interval, -1);
}


void EffectiveSink::Log(const LogMsg& msg) {
  static thread_local std::string buf;
  // 通过Formatter序列化msg到buf中
  formatter_->Format(msg, buf);

  // 如果主缓冲区空，则重置压缩流
  if (master_cache_->Empty()) {
    compress_->ResetStream();
  }
  
  // 压缩并加密后写入主缓存（加锁）
  {
    std::lock_guard<std::mutex> lock(mtx_);
    compressed_buf_.reserve(compress_->CompressedBound(buf.size()));
    size_t compressed_size = compress_->Compress(buf.data(), buf.size(), compressed_buf_.data(), compressed_buf_.capacity());
    
    if (compressed_size == 0) {
      LOG_ERROR("EffectiveSink::Log: compress failed");
      return;
    }

    // 加密
    encryped_buf_.clear();
    encryped_buf_.reserve(compressed_size + 16);  // 预留加密头部容量
    crypt_->Encrypt(compressed_buf_.data(), compressed_size, encryped_buf_);
    if (encryped_buf_.empty()) {
      LOG_ERROR("EffectiveSink::Log: encrypt failed");
      return;
    }
    
    // 写入主缓存
    WriteToCache_(encryped_buf_.data(), encryped_buf_.size());
  }


  // 判断缓存占比是否超过80%，如果超过则将日志写入文件
  if (NeedCacheToFile_()) {
    if (is_slave_free_.load()) {
      is_slave_free_.store(false);
      SwapCache_();
    }
    PrepareToFile_();
  }
}

void EffectiveSink::SetFormatter(std::unique_ptr<Formatter> formatter) {}

// 刷新所有缓存到文件
void EffectiveSink::Flush() {
  TIMER_COUNT("Flush");
  // 从缓存写入文件
  PrepareToFile_();
  WAIT_TASK_IDLE(task_runner_);

  // 如果从缓存空闲，则交换缓存并写入文件
  if (is_slave_free_.load()) {
    is_slave_free_.store(false);
    SwapCache_();
  }
  PrepareToFile_();
  WAIT_TASK_IDLE(task_runner_);
}

// 交换主从缓存
void EffectiveSink::SwapCache_() {
  std::lock_guard<std::mutex> lock(mtx_);
  std::swap(master_cache_, slave_cache_);
}

// 判断主缓存占比是否超过80%
bool EffectiveSink::NeedCacheToFile_() {
  return master_cache_->GetRatio() > 0.8;
}

// 向缓存写入数据
void EffectiveSink::WriteToCache_(const void* data, uint32_t size) {
  detail::ItemHeader item_header;
  item_header.size = size;
  master_cache_->Push(&item_header, sizeof(item_header));
  master_cache_->Push(data, size);
}

// 准备将缓存内容写入文件
void EffectiveSink::PrepareToFile_() {
  POST_TASK(task_runner_, [this]() { CacheToFile_();});
}

// 将缓存内容写入文件
void EffectiveSink::CacheToFile_() {
  TIMER_COUNT("CacheToFile_");
  
  // 如果从缓存为空，则返回
  if (is_slave_free_.load()) {
    return;
  }
  
  // 如果从缓存为空，则标记为空闲并返回
  if (slave_cache_->Empty()) {
    is_slave_free_.store(true);
    return;
  }

  // 将缓存内容写入日志文件
  {
    auto file_path = GetFilePath_();
    detail::ChunkHeader chunk_header;
    chunk_header.size = slave_cache_->Size();

    memcpy(chunk_header.aes_iv, crypt_->GetIv_().data(), crypt_->GetIv_().size());

    std::cout << "aes_iv_: " << chunk_header.aes_iv <<std::endl; 

    memcpy(chunk_header.pub_key, client_pub_key_.data(), client_pub_key_.size());

    std::ofstream ofs(file_path, std::ios::binary | std::ios::app);
    ofs.write(reinterpret_cast<char*>(&chunk_header), sizeof(chunk_header));
    ofs.write(reinterpret_cast<char*>(slave_cache_->Data()), chunk_header.size);
  }

  // 清空从缓存，并标记为空闲
  slave_cache_->Clear();
  is_slave_free_.store(true);
}

// 获取日志文件路径
std::filesystem::path EffectiveSink::GetFilePath_() {
  // 获取当前日期和时间并返回文件路径
  auto GetDateTimePath = [this]() -> std::filesystem::path {
    std::time_t now = std::time(nullptr);  // 获取当前时间戳
    std::tm tm;
    LocalTime(&tm, &now);  
    char time_buf[32] = {0};  
    std::strftime(time_buf, sizeof(time_buf), "%Y%m%d%H%M%S", &tm);  
    return (conf_.dir / (conf_.prefix + "_" + time_buf));  // 拼接目录、文件前缀和时间，生成日志文件路径
  };

  // 如果日志文件路径为空，生成新的日志文件路径
  if (log_file_path_.empty()) {
    log_file_path_ = GetDateTimePath().string() + ".log";  
  } else {
    // 如果日志文件路径已经存在，检查文件大小
    auto file_size = fs::GetFileSize(log_file_path_);
    // 如果文件大小超过配置的单个文件大小限制
    if (file_size > conf_.single_size.count()) {
      std::string date_time_path = GetDateTimePath().string();  
      std::string file_path = date_time_path + ".log";  
      // 如果该文件已经存在，进行文件索引处理
      if (std::filesystem::exists(file_path)) {
        int idx = 0;
        for (auto& path : std::filesystem::directory_iterator(conf_.dir)) {
          // 查找已经存在的文件，并递增索引，确保文件名不重复
          if (path.path().filename().string().find(date_time_path) != std::string::npos) {
            ++idx;
          }
        }
        log_file_path_ = date_time_path + "_" + std::to_string(idx) + ".log";  // 生成新的文件名
      } else {
        log_file_path_ = std::move(file_path);  // 文件路径未存在，直接使用
      }
    }
  }


  LOG_INFO("EffectiveSink::GetFilePath_: log_file_path={}", log_file_path_.string());
  return log_file_path_;  // 返回生成的日志文件路径
}

std::filesystem::path EffectiveSink::GetLog_file_path_() {return log_file_path_;}

// 清理过期的日志文件
void EffectiveSink::ElimateFiles_() {
  LOG_INFO("EffectiveSink::ElimateFiles_: start");  
  std::vector<std::filesystem::path> files;  

  // 遍历日志目录，收集所有扩展名为“.log”的文件
  for (auto& path : std::filesystem::directory_iterator(conf_.dir)) {
    if (path.path().extension() == ".log") {
      files.push_back(path.path());  // 将日志文件路径加入列表
    }
  }

  // 按照文件的最后修改时间排序，从最近的文件开始
  std::sort(files.begin(), files.end(), [](const std::filesystem::path& lhs, const std::filesystem::path& rhs) {
    return std::filesystem::last_write_time(lhs) > std::filesystem::last_write_time(rhs);  // 降序排序
  });

  size_t total_bytes = space_cast<bytes>(conf_.total_size).count();  // 获取配置的总大小限制
  size_t used_bytes = 0;  // 已使用的字节数

  // 遍历文件列表，清理超过总大小限制的旧文件
  for (auto& file : files) {
    used_bytes += fs::GetFileSize(file);  // 累加文件的大小
    // 如果已使用的字节数超过总大小限制，删除文件
    if (used_bytes > total_bytes) {
      LOG_INFO("EffectiveSink::ElimateFiles_: remove file={}", file.string());  // 输出删除文件的信息
      std::filesystem::remove(file);  // 删除文件
    }
  }
}



}  // namespace logger
