#pragma once
#include <iostream>
#include <memory>

#include "log_handle.h"
#include "sinks/sink.h"
#include "sinks/console_sink.h"
#include "log_common.h"

int main(){
    auto sink=std::make_shared<logger::ConsoleSink>();
    logger::LogHandle handle(sink);
    handle.Log(logger::LogLevel::kInfo,logger::SourceLocation(),"test");
    
    return 0;
}