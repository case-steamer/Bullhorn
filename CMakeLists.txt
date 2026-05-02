cmake_minimum_required(VERSION 3.20)

project(Bullhorn VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Bullhorn src/main.cpp src/Driver.cpp src/TimeParser.cpp src/FileSysOp.cpp src/AudioPlayer.cpp)

target_include_directories(Bullhorn PRIVATE include/ libs/)
