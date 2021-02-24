cmake_minimum_required(VERSION 2.8)
project(throttler)

set(CMAKE_CXX_STANDARD 11)

#include_directories(/usr/include/mysql/)
link_directories(/usr/lib64/mysql/)
add_executable(throttler main.cpp cnfMng.cpp func.cpp handleThread.cpp handleThread.h)
target_link_libraries(throttler mysqlclient)
link_directories(/usr/lib64/)
target_link_libraries(throttler pthread)