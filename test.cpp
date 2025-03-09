#include <iostream>

#include <chrono>

#include "log_handle.h"
#include "sinks/console_sink.h"
#include "sinks/effective_sink.h"
#include "decode/decode.h"

std::string GenerateRandomString(int length) {
    std::string str;
    str.reserve(length);
    for (int i = 0; i < length; ++i) {
      str.push_back('a' + rand() % 26);
    }
    return str;
  }

int main(){
    
  std::cout << "Logger Example Start!" << std::endl;
  std::shared_ptr<logger::LogSink> sink = std::make_shared<logger::ConsoleSink>();
  logger::EffectiveSink::Conf conf;
  conf.dir = "C:\\Users\\Lawliet\\Desktop\\log_test\\logger_test";
  conf.prefix = "loggerdemo";

  auto ecdh_key = logger::crypt::GenECDHKey();
  auto sever_pri_key = std::get<0>(ecdh_key);
  conf.pub_key = std::get<1>(ecdh_key);

  {
    std::shared_ptr<logger::LogSink> effective_sink = std::make_shared<logger::EffectiveSink>(conf);
    logger::LogHandle handle({effective_sink});

    //std::string str =  GenerateRandomString(500);
    std::string str = "Hello MyLog!";

    auto begin = std::chrono::system_clock::now();
    // for (int i = 0; i < 1000000; ++i) {
    //   if (i % 100000 == 0) {
    //     std::cout << "i " << i << std::endl;
    //   }
    //   handle.Log(logger::LogLevel::kInfo, logger::SourceLocation(), str);
    // }
    std::cout << "log: " << str << std::endl;
    
    handle.Log(logger::LogLevel::kInfo, logger::SourceLocation(), str);
    effective_sink->Flush();
    auto end = std::chrono::system_clock::now();
    std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "our logger diff: " << diff.count() << std::endl;

    std::cout << "file path: " << effective_sink->GetLog_file_path_().string() << std::endl;


    std::string input_file_path =  effective_sink->GetLog_file_path_().string();
    std::string output_file_path = "C:\\Users\\Lawliet\\Desktop\\log_test\\logger_test\\hello.txt";
    try {
      decode_formatter = std::make_unique<DecodeFormatter>();
      decode_formatter->SetPattern("[%l][%D:%S][%p:%t][%F:%f:%#]%v");
      decompress = std::make_unique<logger::compress::ZstdCompress>();
      DecodeFile(input_file_path, sever_pri_key, output_file_path);
    } catch (const std::exception& e) {
      std::cerr << "Decode failed: " << e.what() << std::endl;
      return 1;
    }

    }

    std::cout << "Logger Example End!" << std::endl;

    return 0;
}