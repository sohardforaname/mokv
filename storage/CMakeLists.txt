cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_STORAGE)

add_library(STORAGE ${DIR_STORAGE})

add_executable(SKIP_LIST_TEST skip_list.cc ./../util/arena.cc)

add_executable(BUFFER_POOL_TEST buffer_pool.cc buffer_pool_test.cc)