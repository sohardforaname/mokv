cmake_minimum_required(VERSION 3.16)
project(moKV)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(STORAGE)
add_subdirectory(UTIL)
add_executable(moKV main.cpp STORAGE UTIL)