#include <filesystem>   // 处理文件路径
#include <fstream>      // 文件输入输出流
#include <iostream>     // 标准输入输出
#include <memory>       // 智能指针
#include <streambuf>    // 处理流缓冲区
#include <string>       // 处理字符串
#include <vector>       // 处理动态数组

// 引入日志解析和处理相关头文件
#include "decode_formatter.h"        // 解析日志格式
#include "compress/zstd_compress.h"  // Zstd 压缩解压缩
#include "crypt/aes_crypt.h"         // AES 加密解密
#include "sinks/effective_sink.h"    // 日志处理工具

using namespace logger;
using namespace logger::detail;

// 全局日志解码格式化对象
std::unique_ptr<DecodeFormatter> decode_formatter;

// **函数: 读取二进制文件**
std::vector<char> ReadFile(const std::string& file_path) {
  std::ifstream ifs(file_path, std::ios::binary); // 以二进制方式读取文件
  if (!ifs) {
    throw std::runtime_error("ReadFile: open file failed");
  }
  ifs.seekg(0, std::ios::end);  // 移动文件指针到文件末尾
  auto size = ifs.tellg();      // 获取文件大小
  ifs.seekg(0, std::ios::beg);  // 重新定位到文件开始
  std::vector<char> buffer(size); // 创建缓冲区存储文件内容
  ifs.read(buffer.data(), size);  // 读取文件内容到缓冲区
  return buffer;
}

// **函数: 追加数据到文件**
void AppendDataToFile(const std::string& file_path, const std::string& data) {
  std::ofstream ofs(file_path, std::ios::binary | std::ios::app);
  ofs.write(data.data(), data.size()); // 追加写入数据
}

// **全局解压缩对象**
std::unique_ptr<logger::compress::Compression> decompress;

// **函数: 解析单个数据项并解密**
void DecodeItemData(char* data, size_t size, crypt::Crypt* crypt, std::string& output) {
  std::string decrypted = crypt->Decrypt(data, size); // 使用 AES 进行数据解密
  std::string decompressed = decompress->Decompress(decrypted.data(), decrypted.size()); // 使用 Zstd 进行数据解压缩
  EffectiveMsg msg;
  msg.ParseFromString(decompressed); // 解析数据项
  std::string assemble;
  decode_formatter->Format(msg, assemble); // 格式化输出日志数据
  output.append(assemble);
}

// **函数: 解析数据块**
void DecodeChunkData(char* data,
                     size_t size,
                     const std::string& cli_pub_key, // 客户端公钥
                     const std::string& svr_pri_key, // 服务器私钥
                     std::string& output,
                     const std::string& iv) {
  std::cout << "decode chunk " << size << std::endl;

  // **使用 ECDH 计算共享密钥**
  //std::string svr_pri_key_bin = crypt::HexKeyToBinary(svr_pri_key); // 私钥转换为二进制格式
  std::string shared_secret = crypt::GenECDHSharedSecret(svr_pri_key, cli_pub_key); // 计算共享密钥

  // **创建 AES 解密对象**
  std::unique_ptr<crypt::Crypt> crypt = std::make_unique<crypt::AESCrypt>(shared_secret);
  
  crypt->SetIv_(iv);

  size_t offset = 0;
  size_t count = 0;

  // **解析数据项**
  while (offset < size) {
    ++count;
    if (count % 1000 == 0) {
      std::cout << "decode item " << count << std::endl;
    }
    
    // 解析数据项头部
    ItemHeader* item_header = reinterpret_cast<ItemHeader*>(data + offset);
    if (item_header->magic != ItemHeader::kMagic) {
      throw std::runtime_error("DecodeChunkData: invalid item magic");
      return;
    }
    
    offset += sizeof(ItemHeader);
    
    // **解析并解密数据项**
    DecodeItemData(data + offset, item_header->size, crypt.get(), output);
    
    offset += item_header->size;
    output.push_back('\n'); // 添加换行符
  }
}

// **函数: 解码整个日志文件**
void DecodeFile(const std::string& input_file_path, const std::string& pri_key, const std::string& output_file_path) {
  auto input = ReadFile(input_file_path); // 读取输入文件
  if (input.size() < sizeof(ChunkHeader)) {
    throw std::runtime_error("DecodeFile: input file is too small");
    return;
  }

  // 解析并校验文件头
  auto chunk_header = reinterpret_cast<ChunkHeader*>(input.data());
  if (chunk_header->magic != ChunkHeader::kMagic) {
    throw std::runtime_error("DecodeFile: invalid file magic");
    return;
  }

  size_t offset = 0;
  size_t file_size = input.size();
  std::string output;
  output.reserve(1024 * 1024); // 预分配 1MB 空间

  // **遍历所有数据块并解码**
  while (offset < file_size) {
    ChunkHeader* chunk_header = reinterpret_cast<ChunkHeader*>(input.data() + offset);
    if (chunk_header->magic != ChunkHeader::kMagic) {
      throw std::runtime_error("DecodeFile: invalid chunk magic");
      return;
    }
    
    output.clear();
    offset += sizeof(ChunkHeader);
    
    // **解析并解密数据块**
    DecodeChunkData(input.data() + offset, chunk_header->size, std::string(chunk_header->pub_key, 65), pri_key, output,
     std::string(chunk_header->aes_iv));
    
    offset += chunk_header->size;
    
    // **追加解码后的数据到输出文件**
    AppendDataToFile(output_file_path, output);
  }
}

// **主函数已被注释掉**
// 该代码片段的 `main` 函数被注释掉了，通常 `main` 用于执行程序，解析命令行参数并调用 `DecodeFile` 进行解码。
// main 函数示例:
// int main(int argc, char* argv[]) {
//   if (argc != 4) {
//     std::cerr << "Usage: ./decode <file_path> <pri_key> <output_file>" << std::endl;
//     return 1;
//   }
//   std::string input_file_path = argv[1];
//   std::string pri_key = argv[2];
//   std::string output_file_path = argv[3];
//   try {
//     decode_formatter = std::make_unique<DecodeFormatter>();
//     decode_formatter->SetPattern("[%l][%D:%S][%p:%t][%F:%f:%#]%v");
//     decompress = std::make_unique<logger::compress::ZstdCompress>();
//     DecodeFile(input_file_path, pri_key, output_file_path);
//   } catch (const std::exception& e) {
//     std::cerr << "Decode failed: " << e.what() << std::endl;
//     return 1;
//   }
//   return 0;
// }
