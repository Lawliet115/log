#pragma once
#include <iostream>
#include <memory>

#include "log_handle.h"
#include "sinks/sink.h"
#include "sinks/console_sink.h"
#include "log_common.h"
#include"internal_log.h"

int main(){
    
    #ifdef ENABLE_LOG
    LOG_INFO("begin log");
    #endif

    auto sink=std::make_shared<logger::ConsoleSink>();
    logger::LogHandle handle(sink);
    handle.Log(logger::LogLevel::kInfo,logger::SourceLocation("test",1,"test.111"),"test");


    return 0;
}