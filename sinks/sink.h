#pragma once

#include <memory>
#include <filesystem>

#include "formatter/formatter.h"
#include "log_common.h"
#include "log_msg.h"

namespace logger {
class LogSink {
 public:
  virtual ~LogSink() = default;

  virtual void Log(const LogMsg& msg) = 0;

  virtual void SetFormatter(std::unique_ptr<Formatter> formatter) = 0;

  virtual void Flush() {}

  virtual std::filesystem::path GetLog_file_path_() = 0;

};
}  // namespace logger
