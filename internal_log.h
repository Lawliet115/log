#pragma once

#define ENABLE_LOG 1

#ifdef ENABLE_LOG

#include "fmt/core.h"

//...为可变参数
//__FILE__ 当前源文件的名称
// __LINE__ 当前源文件中的行号
//__VA_ARGS__ 是一个特殊的宏参数，代表传递给宏的所有可变参数。
//Log传入基本信息，fmt::format将传入的信息拼接出来


#define LOG_INFO(...) \
  fmt::print("[INFO] [{}:{}] {} {}\n", __FILE__, __LINE__, std::chrono::system_clock::now(), fmt::format(__VA_ARGS__))

#define LOG_DEBUG(...) \
  fmt::print("[DEBUG] [{}:{}] {} {}\n", __FILE__, __LINE__, std::chrono::system_clock::now(), fmt::format(__VA_ARGS__))

#define LOG_ERROR(...) \
  fmt::print("[ERROR] [{}:{}] {} {}\n", __FILE__, __LINE__, std::chrono::system_clock::now(), fmt::format(__VA_ARGS__))

#else

#define LOG_INFO(...)

#define LOG_DEBUG(...)

#define LOG_ERROR(...)

#endif
