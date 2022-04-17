link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(ARENA_TEST arena_test.cc)
target_link_libraries(ARENA_TEST libmokv.a)

add_executable(FILE_CACHE_TEST file_cache_test.cc)
target_link_libraries(FILE_CACHE_TEST libmokv.a)

add_executable(FILE_META_TEST file_meta_test.cc)
target_link_libraries(FILE_META_TEST libmokv.a)

add_executable(LRU_CACHE_TEST lru_cache_test.cc)
target_link_libraries(LRU_CACHE_TEST libmokv.a)

add_executable(SKIP_LIST_TEST skip_list_test.cc)
target_link_libraries(SKIP_LIST_TEST libmokv.a)

add_executable(SLICE_TEST slice_test.cc)
target_link_libraries(SLICE_TEST libmokv.a)

add_executable(TABLE_TEST table_test.cc)
target_link_libraries(TABLE_TEST libmokv.a)