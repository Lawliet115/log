#include "formatter/default_formatter.h"
#include"utils/sys_util.h"

namespace logger {

const std::vector<std::string> KLogValueStr{
  "Trace", "Debug", "Info", "Warn", "Error", "Fatal","Off" 
};

//system_clock fmt可以格式化
void DefaultFormatter::Format(const LogMsg& msg, MemoryBuf& dest) {
dest = fmt::format("[{0:%Y-%m-%d %H:%M:%S}] [{1}] [{2}:{3}] [{4}:{5}] {6}", 
  std::chrono::system_clock::now(),
  KLogValueStr[static_cast<int>(msg.level)],
  msg.location.file_name.data(),
  msg.location.line,
  logger::GetProcessId(),
  logger::GetThreadId(),
  msg.message.data()
);
}

}  // namespace logger
