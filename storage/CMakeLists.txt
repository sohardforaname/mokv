cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_STORAGE)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(STORAGE ${DIR_STORAGE})

add_executable(SKIP_LIST_TEST skip_list_test.cc)
target_link_libraries(SKIP_LIST_TEST libUTIL.a)

add_executable(BUFFER_POOL_TEST buffer_pool.cc buffer_pool_test.cc)