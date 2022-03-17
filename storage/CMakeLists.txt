cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_STORAGE)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(storage ${DIR_STORAGE})

add_executable(skip_list_test skip_list_test.cc)
target_link_libraries(skip_list_test libutil.a)

add_executable(buffer_pool_test buffer_pool.cc buffer_pool_test.cc)