cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "logger")
project(PROJECT_NAME)

include_directories("C:/Users/Lawliet/Desktop/log") # 头文件目录

set(CMAKE_CXX_STANDARD 17)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(UTIL_SRCS utils/sys_util_linux.cc utils/sys_util.cc utils/file_util.cc)
    set(MMAP_SRCS mmap/mmap.cpp mmap/mmap_linux.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(UTIL_SRCS utils/sys_util_win.cc utils/sys_util.cc utils/file_util.cc)
    set(MMAP_SRCS mmap/mmap.cpp mmap/mmap_win.cpp)
else()
    message(FATAL_ERROR "unsupported.")
endif()

set(COMPRESS_SRCS compress/zlib_compress.cpp compress/zstd_compress.cpp)
set(CONTEXT_SRCS context/context.cpp context/executor.cpp context/thread_pool.cpp)
set(FORMATTER_SRCS formatter/effective_formatter.cpp formatter/default_formatter.cpp)
set(SINK_SRCS sinks/console_sink.cpp )
set(PROTO_SRCS proto/effective_msg.pb.cc)

set(SRCS
    log_handle.cpp
    log_msg.cpp
    ${FORMATTER_SRCS}
    ${SINK_SRCS}
    ${UTIL_SRCS}
    ${MMAP_SRCS}
    ${CONTEXT_SRCS}
    ${COMPRESS_SRCS}
    ${PROTO_SRCS}
)

add_executable(${PROJECT_NAME} test.cpp ${SRCS}) # 生成可执行文件，这里程序名即为功能名

# 从环境变量中读取 ENABLE_LOG 的值
set(ENABLE_LOG $ENV{ENABLE_LOG})
# 根据环境变量的值添加宏定义
if(ENABLE_LOG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_LOG)
endif()



find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

find_package(zstd CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE zstd::libzstd)

find_package(Protobuf REQUIRED)
target_link_libraries(${PROJECT_NAME}  PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)



