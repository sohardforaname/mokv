cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_DB)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(db ${DIR_DB})

add_executable(table_test table_test.cc table.cc)
target_link_libraries(table_test libstorage.a libutil.a)
