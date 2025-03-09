#include <iostream>
#include "sinks/console_sink.h"

#include "formatter/default_formatter.h"
#include "formatter/effective_formatter.h"

namespace logger {

ConsoleSink::ConsoleSink() : formatter_(std::make_unique<DefaultFormatter>()) {}
//ConsoleSink::ConsoleSink() : formatter_(std::make_unique<EffectiveFormatter>()) {}

void ConsoleSink::Log(const LogMsg& msg) {
  fmt::print("ConsoleSink Log:\n");
  std::string buf;
  fmt::print("format:{}\n",buf);
  formatter_->Format(msg, buf);
  //fmt::print("format:{}\n",buf);
  std::cout << buf;
}

void ConsoleSink::SetFormatter(std::unique_ptr<Formatter> formatter) {
  formatter_ = std::move(formatter);
}

std::filesystem::path ConsoleSink::GetLog_file_path_() {return std::filesystem::path();}

}  // namespace logger
