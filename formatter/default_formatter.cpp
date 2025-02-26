#include "formatter/default_formatter.h"


namespace logger {

void DefaultFormatter::Format(const LogMsg& msg, MemoryBuf* dest) {
  
  dest->append(msg.message.data(), msg.message.size());
}

}  // namespace logger
