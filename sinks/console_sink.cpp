#include "sinks/console_sink.h"

#include "formatter/default_formatter.h"

namespace logger {

ConsoleSink::ConsoleSink() : formatter_(std::make_unique<DefaultFormatter>()) {}

void ConsoleSink::Log(const LogMsg& msg) {
  fmt::print("ConsoleSink Log:\n");
  std::string buf;
  formatter_->Format(msg, buf);
  fmt::print("format:{}\n",buf);
}

void ConsoleSink::SetFormatter(std::unique_ptr<Formatter> formatter) {
  formatter_ = std::move(formatter);
}

}  // namespace logger
