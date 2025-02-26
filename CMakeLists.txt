cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "logger")

include_directories("C:/Users/Lawliet/Desktop/log") # 头文件目录

set(CMAKE_CXX_STANDARD 17)


set(FORMATTER_SRCS formatter/default_formatter.cpp)
set(SINK_SRCS sinks/console_sink.cpp )

set(SRCS
    log_handle.cpp
    log_msg.cpp
    ${FORMATTER_SRCS}
    ${SINK_SRCS}
)

add_executable(${PROJECT_NAME} test.cpp ${SRCS}) # 生成可执行文件，这里程序名即为功能名

