#pragma once

#include <atomic>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <iostream>

#include <vector>

#include "log_common.h"
#include "log_msg.h"
#include "sinks/sink.h"

namespace logger
{
class Sink;//前置声明、pimpl模式
using SinkPtr = std::shared_ptr<Sink>;
using SinkPtrInitList = std::initializer_list<SinkPtr>;

class Logger
{
public:
  explicit Logger(std::string name, SinkPtr sink);
  explicit Logger(std::string name, SinkPtrInitList sinks);
  explicit Logger(Logger &&other) noexcept;
  Logger& operator= (Logger other) noexcept;

  //根据存储sink的容器迭代器构造
  template <typename It>
  Logger(std::string name, It begin, It end) : Logger(name, SinkPtrInitList(begin, end)) {}

  //虚析构
  ~Logger();

  //禁用拷贝构造和赋值
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void SetLevel(LogLevel level);

  LogLevel GetLevel() const;

  void Log(LogLevel level, SourceLocation loc, StringView message);

protected:
  bool ShouldLog_(LogLevel level) const noexcept {
      return level >= level_ && !sinks_.empty(); 
  }

  virtual void Log_(const LogMsg& msg);

 private:
  std::string name_;
  std::atomic<LogLevel> level_;
  std::vector<SinkPtr> sinks_;
};

} // namespace logger
