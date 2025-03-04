#pragma once
#include <iostream>
#include <memory>

#include "log_handle.h"
#include "sinks/sink.h"
#include "sinks/console_sink.h"
#include "log_common.h"
#include "internal_log.h"
#include "utils/sys_util.h"
#include "utils/file_util.h"
#include "mmap/mmap.h"
#include "context/context.h"
#include "compress/zstd_compress.h"
#include "compress/zlib_compress.h"  


int main(){
    
    fmt::print("-------------------\n");
    auto sink=std::make_shared<logger::ConsoleSink>();
    logger::LogHandle handle(sink);
    handle.Log(logger::LogLevel::kInfo,logger::SourceLocation("test",1,"test.111"),"test");
    fmt::print("-------------------\n");


    //context test
    fmt::print("-------------------\n");
    fmt::print("begin context test:\n");

    logger::ctx::TaskRunnerTag tag = NEW_TASK_RUNNER(10086);

    POST_TASK(tag, [] {
      fmt::print("POST_TASK\n");
    });

    POST_REPEATED_TASK(tag, [] {
        fmt::print("POST_REPEATED_TASK\n");
    }, std::chrono::microseconds(100), 3);

    POST_DELAY_TASK(tag, [] {
        fmt::print("POST_DELAY_TASK\n");
    }, std::chrono::microseconds(100));

    std::this_thread::sleep_for(std::chrono::seconds(2));


    //compress test
    fmt::print("-------------------\n");
    std::string input = "This is a test string for compression and decompression.";

    std::string large_input;
    for (int i = 0; i < 100; i++)
    {
        large_input += input;  
    }
   
    {
        logger::compress::ZstdCompress zstd_compressor;
        // 压缩数据
        auto start = std::chrono::high_resolution_clock::now();
        size_t input_size = large_input.size();  // 使用 large_input 进行测试
        size_t output_size = zstd_compressor.CompressedBound(input_size);
        std::vector<uint8_t> compressed_data(output_size);
        size_t compressed_size = zstd_compressor.Compress(large_input.data(), input_size, compressed_data.data(), output_size);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> compress_duration = end - start;

        if (compressed_size == 0) {
            std::cerr << "Zstd Compression failed" << std::endl;
            return 1;
        }
        compressed_data.resize(compressed_size);

        // 打印压缩后的数据长度和内容的一部分（如果数据量大，可以打印前几字节）
        std::cout << "Zstd Compression finished. Compressed size: " << compressed_size << " bytes." << std::endl;
        std::cout << "Compressed data (first 100 bytes): ";
        for (size_t i = 0; i < 100 && i < compressed_data.size(); ++i) {
            std::cout << std::hex << (int)compressed_data[i] << " ";
        }
        std::cout << std::dec << std::endl;

        // 解压数据
        start = std::chrono::high_resolution_clock::now();
        std::string decompressed_data = zstd_compressor.Decompress(compressed_data.data(), compressed_size);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> decompress_duration = end - start;

        if (decompressed_data.empty()) {
            std::cerr << "Zstd Decompression failed" << std::endl;
            return 1;
        }

        // 打印解压后的数据内容（如果数据量大，只打印前 100 个字符）
        std::cout << "Zstd Decompression finished. Decompressed size: " << decompressed_data.size() << " bytes." << std::endl;
        std::cout << "Decompressed data (first 100 characters): " << decompressed_data.substr(0, 100) << std::endl;

        std::cout << "Zstd Compression time: " << compress_duration.count() << " ms" << std::endl;
        std::cout << "Zstd Decompression time: " << decompress_duration.count() << " ms" << std::endl;
    }
    fmt::print("-------------------\n");
    {
        logger::compress::ZlibCompress zlib_compressor;

        // 压缩数据
        auto start = std::chrono::high_resolution_clock::now();
        zlib_compressor.ResetStream();
        size_t input_size = large_input.size();  // 使用 large_input 进行测试
        size_t output_size = zlib_compressor.CompressedBound(input_size);
        std::vector<uint8_t> compressed_data(output_size);
        size_t compressed_size = zlib_compressor.Compress(large_input.data(), input_size, compressed_data.data(), output_size);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> compress_duration = end - start;

        if (compressed_size == 0) {
            std::cerr << "Zlib Compression failed" << std::endl;
            return 1;
        }
        compressed_data.resize(compressed_size);

        // 打印压缩后的数据长度和内容的一部分
        std::cout << "Zlib Compression finished. Compressed size: " << compressed_size << " bytes." << std::endl;
        std::cout << "Compressed data (first 100 bytes): ";
        for (size_t i = 0; i < 100 && i < compressed_data.size(); ++i) {
            std::cout << std::hex << (int)compressed_data[i] << " ";
        }
        std::cout << std::dec << std::endl;

        // 解压数据
        start = std::chrono::high_resolution_clock::now();
        std::string decompressed_data = zlib_compressor.Decompress(compressed_data.data(), compressed_size);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> decompress_duration = end - start;

        if (decompressed_data.empty()) {
            std::cerr << "Zlib Decompression failed" << std::endl;
            return 1;
        }

        // 打印解压后的数据内容（如果数据量大，只打印前 100 个字符）
        std::cout << "Zlib Decompression finished. Decompressed size: " << decompressed_data.size() << " bytes." << std::endl;
        std::cout << "Decompressed data (first 100 characters): " << decompressed_data.substr(0, 100) << std::endl;

        std::cout << "Zlib Compression time: " << compress_duration.count() << " ms" << std::endl;
        std::cout << "Zlib Decompression time: " << decompress_duration.count() << " ms" << std::endl;
    }

    return 0;
}