cmake_minimum_required(VERSION 3.16)
project(moKV)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
 
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories (
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/db
    ${PROJECT_SOURCE_DIR}/include/storage
    ${PROJECT_SOURCE_DIR}/include/util
)

link_directories (
    ${LIBRARY_OUTPUT_DIRECTORY}
)

add_subdirectory(src)
add_subdirectory(test)