cmake_minimum_required(VERSION 3.16)

aux_source_directory(. DIR_DB)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(DB ${DIR_DB})

add_executable(TABLE_TEST table_test.cc table.cc)
target_link_libraries(TABLE_TEST libSTORAGE.a libUTIL.a)
