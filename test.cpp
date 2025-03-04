#pragma once
#include <iostream>
#include <memory>

#include "formatter/default_formatter.h"
#include "formatter/effective_formatter.h"
#include "proto/effective_msg.pb.h"
#include "utils/sys_util.h"
#include "utils/file_util.h"

#include "log_handle.h"
#include "sinks/sink.h"
#include "sinks/console_sink.h"
#include "log_common.h"
#include "internal_log.h"
#include "mmap/mmap.h"
#include "context/context.h"
#include "compress/zstd_compress.h"
#include "compress/zlib_compress.h"  


int main(){
    
    fmt::print("-------------------\n");
    // auto sink = std::make_shared<logger::ConsoleSink>();
    // logger::LogHandle handle(sink);
    // handle.Log(logger::LogLevel::kInfo,logger::SourceLocation("test",1,"test.111"),"test");
    // fmt::print("-------------------\n");

    auto sink = std::make_shared<logger::ConsoleSink>();
    auto fmt = std::make_unique<logger::EffectiveFormatter>();

    sink->SetFormatter(std::move(fmt));

    logger::LogHandle handle({sink});
    handle.Log(logger::LogLevel::kInfo, logger::SourceLocation("proto_test.txt", 100, "test.cpp"), "this is proto test");


    // logger::SourceLocation sl("test.cpp",100,"func_name_in");
    // std::string msgstr= "hello logger";
    // logger::LogMsg log_msg(sl,logger::LogLevel::kInfo,msgstr);
    // logger::EffectiveFormatter eff;
    // std::string eff_str;
    // eff.Format(log_msg,eff_str);
    
    // EffectiveMsg new_eff;
    // if(!new_eff.ParseFromArray(eff_str.data(),eff_str.size())){
    //     std::cerr<<" parse error"<<std::endl;
    //     return -1;
    // }
    // std::cout<<new_eff.level()<<"\n";

    // std::cout<<eff_str<<"\n";
    return 0;
}