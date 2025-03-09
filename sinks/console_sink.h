#pragma once

#include "sinks/sink.h"
#include <filesystem>

namespace logger {

class ConsoleSink : public LogSink {
 public:
  ConsoleSink();

  ~ConsoleSink() override = default;

  void Log(const LogMsg& msg) override;

  void SetFormatter(std::unique_ptr<Formatter> formatter) override;

  std::filesystem::path GetLog_file_path_() override;

 private:
  std::unique_ptr<Formatter> formatter_;
};

}  // namespace logger
