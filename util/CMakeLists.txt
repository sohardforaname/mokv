cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_UTIL)

add_library(util ${DIR_UTIL})

add_executable(arena_test arena.cc arena_test.cc)

add_executable(lru_test lru_cache_test.cc)

add_executable(slice_test slice_test.cc)