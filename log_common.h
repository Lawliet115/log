#pragma once

#include <string.h>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>


#define LOGGER_LEVEL_TRACE 0
#define LOGGER_LEVEL_DEBUG 1
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 3
#define LOGGER_LEVEL_ERROR 4
#define LOGGER_LEVEL_CRITICAL 5
#define LOGGER_LEVEL_OFF 6

namespace logger {

using StringView = std::string_view;
using MemoryBuf = std::string;

enum class LogLevel {
  kTrace = LOGGER_LEVEL_TRACE,
  kDebug = LOGGER_LEVEL_DEBUG,
  kInfo = LOGGER_LEVEL_INFO,
  kWarn = LOGGER_LEVEL_WARN,
  kError = LOGGER_LEVEL_ERROR,
  kFatal = LOGGER_LEVEL_CRITICAL,
  kOff = LOGGER_LEVEL_OFF
};

#define LOGGER_ACTIVE_LEVEL LOGGER_LEVEL_TRACE

struct SourceLocation {
  constexpr SourceLocation() = default;

  SourceLocation(StringView file_name_in, int32_t line_in, StringView func_name_in)
      : file_name{file_name_in}, line{line_in}, func_name{func_name_in} {
    // file_name_in is a full path, we only need the file name
    // get the file name by finding the last slash
    if (!file_name.empty()) {
      size_t pos = file_name.rfind('/');
      if (pos != StringView::npos) {
        file_name = file_name.substr(pos + 1);
      } else {
        pos = file_name.rfind('\\');
        if (pos != StringView::npos) {
          file_name = file_name.substr(pos + 1);
        }
      }
    }
  }

  StringView file_name;
  int32_t line{0};
  StringView func_name;
};

}  // namespace logger
