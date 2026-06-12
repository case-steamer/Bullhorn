cmake_minimum_required(VERSION 3.20)

#READ libs/CMakeLists.txt FOR DEPENDENCY INSTRUCTIONS

project(Bullhorn VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Bullhorn)

add_subdirectory(src)
add_subdirectory(libs)

target_compile_definitions(Bullhorn PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(Bullhorn PRIVATE include/ include/dataclasses/ libs/)
