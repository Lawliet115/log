#include "compress/zlib_compress.h"

namespace logger {
namespace compress {

// 判断数据是否已压缩
static bool IsCompressed(const void* input, size_t input_size) {
  if (!input) {
    return false;  // 输入为空，直接返回 false
  }

  if (input_size < 2) {
    return false;  // 输入数据太小，无法判断是否压缩
  }

  uint16_t magic = *(uint16_t*)input;  // 获取数据流前两个字节
  // 判断魔数（魔术数），如果是已知的压缩格式魔数，则认为数据已压缩
  if (magic == 0x9c78 || magic == 0xda78 || magic == 0x5e78 || magic == 0x0178) {
    return true;  
  }

  return false;  
}

// 压缩函数
size_t ZlibCompress::Compress(const void* input, size_t input_size, void* output, size_t output_size) {
  if (!input || !output) {
    return 0;  // 输入或输出为空，返回 0 表示压缩失败
  }

  if (!compress_stream_) {
    return 0;  // 压缩流未初始化，返回 0
  }

  // 设置输入输出流指针和大小
  compress_stream_->next_in = (Bytef*)input;
  compress_stream_->avail_in = static_cast<uInt>(input_size);

  compress_stream_->next_out = (Bytef*)(output);
  compress_stream_->avail_out = static_cast<uInt>(output_size);

  int ret = Z_OK;
  do {
    // 调用 deflate 压缩函数，使用 Z_SYNC_FLUSH 标志来同步刷新缓冲区
    ret = deflate(compress_stream_.get(), Z_SYNC_FLUSH);
    // 判断返回值是否为正常压缩结束或缓冲区空间不足，若是其他错误则返回 0
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_STREAM_END) {
      return 0;  // 压缩出错
    }
  } while (ret == Z_BUF_ERROR);  // 如果是 Z_BUF_ERROR，表示缓冲区空间不足，继续压缩

  // 计算实际压缩后的数据大小
  size_t out_len = output_size - compress_stream_->avail_out;
  return out_len;  // 返回压缩后的数据大小
}

// 解压缩函数
std::string ZlibCompress::Decompress(const void* input, size_t input_size) {
  if (!input) {
    return "";  // 输入为空，返回空字符串
  }

  // 判断数据是否已压缩，如果已压缩则初始化解压缩流
  if (IsCompressed(input, input_size)) {
    ResetUncompressStream_();  // 重置解压缩流
  }

  if (!uncompress_stream_) {
    return "";  // 解压缩流初始化失败，返回空字符串
  }

  // 设置解压缩流的输入数据和大小
  uncompress_stream_->next_in = (Bytef*)input;
  uncompress_stream_->avail_in = static_cast<uInt>(input_size);

  std::string output;
  // 一直解压直到没有数据
  while (uncompress_stream_->avail_in > 0) {
    char buffer[4096] = {0};  // 解压缩的缓冲区
    uncompress_stream_->next_out = (Bytef*)buffer;
    uncompress_stream_->avail_out = sizeof(buffer);

    // 调用 inflate 函数进行解压
    int ret = inflate(uncompress_stream_.get(), Z_SYNC_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
      return "";  // 解压缩出错，返回空字符串
    }

    // 将解压后的数据添加到输出字符串中
    output.append(buffer, sizeof(buffer) - uncompress_stream_->avail_out);
  }

  return output;  // 返回解压缩后的数据
}

// 初始化压缩流
void ZlibCompress::ResetStream() {
  compress_stream_ = std::unique_ptr<z_stream, ZStreamDeflateDeleter>(new z_stream());
  compress_stream_->zalloc = Z_NULL;
  compress_stream_->zfree = Z_NULL;
  compress_stream_->opaque = Z_NULL;

  // 初始化压缩流，使用最佳压缩级别，最大窗口大小
  int32_t ret = deflateInit2(compress_stream_.get(), Z_BEST_COMPRESSION, Z_DEFLATED, MAX_WBITS, MAX_MEM_LEVEL,
                             Z_DEFAULT_STRATEGY);
  if (ret != Z_OK) {
    compress_stream_.reset();  // 如果初始化失败，重置流
  }
}

// 初始化解压缩流
void ZlibCompress::ResetUncompressStream_() {
  uncompress_stream_ = std::unique_ptr<z_stream, ZStreamInflateDeleter>(new z_stream());
  uncompress_stream_->zalloc = Z_NULL;
  uncompress_stream_->zfree = Z_NULL;
  uncompress_stream_->opaque = Z_NULL;
  uncompress_stream_->avail_in = 0;
  uncompress_stream_->next_in = Z_NULL;

  // 初始化解压缩流
  int32_t ret = inflateInit2(uncompress_stream_.get(), MAX_WBITS);
  if (ret != Z_OK) {
    uncompress_stream_.reset();  // 如果初始化失败，重置流
  }
}

// 返回压缩后的数据所需的最大缓冲区大小
size_t ZlibCompress::CompressedBound(size_t input_size) {
  return input_size + 10;  // 给出一个最大值
}

}  // namespace compress
}  // namespace logger
