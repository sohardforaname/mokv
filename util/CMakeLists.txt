cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_UTIL)

add_library(UTIL ${DIR_UTIL})

add_executable(BINARY_TEST binary_test.cc)

add_executable(ARENA_TEST arena.cc arena_test.cc)

add_executable(LRU_TEST lru_cache_test.cc)