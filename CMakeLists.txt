cmake_minimum_required(VERSION 3.2)

project(wxbot
    LANGUAGES C CXX #定义编程语言
)

#分别指定源码和可执行文件的编码
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>") #使用系统默认代码页GBK来兼容xxA函数

#定义如何链接VC运行时
add_compile_options($<$<CONFIG:Debug>:/MDd>) #MSVCRTD.lib
add_compile_options($<$<CONFIG:Release>:/MT>) #LIBCMT.lib

file(GLOB_RECURSE sources RELATIVE ${CMAKE_HOME_DIRECTORY} src/*.c src/*.cpp)
message(STATUS "编译代码: ${sources}")
add_executable(wxbot ${sources})
# add_library(wxbot STATIC ${sources})
