cmake_minimum_required(VERSION 3.0)
project(SekaiEngine)

option(RUN_TEST "RUN_TEST" OFF)
option(BUILD_DOC "Build doc using Doxygen" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_subdirectory("${CMAKE_SOURCE_DIR}/engine")
add_subdirectory("${CMAKE_SOURCE_DIR}/sandbox")
if(${RUN_TEST})
    enable_testing()
    add_subdirectory("${CMAKE_SOURCE_DIR}/test")
endif()

if(${BUILD_DOC})
    add_subdirectory("${CMAKE_SOURCE_DIR}/doc")
endif()