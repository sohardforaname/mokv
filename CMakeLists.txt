cmake_minimum_required(VERSION 3.16)
project(moKV)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(STORAGE)
add_subdirectory(UTIL)
add_subdirectory(DB)
add_executable(moKV main.cpp STORAGE UTIL DB)