cmake_minimum_required(VERSION 3.5)
project(SekaiEngine)

option(RUN_TEST "RUN_TEST" OFF)
option(BUILD_DOC "Build doc using Doxygen" OFF)
option(BUILD_EXAMPLES "Build engine examples" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

set(PROJECT_BUILD_EXAMPLE ${BUILD_EXAMPLES})
unset(BUILD_EXAMPLES)
add_subdirectory("${CMAKE_SOURCE_DIR}/engine")

if(${PROJECT_BUILD_EXAMPLES})
    add_subdirectory("${CMAKE_SOURCE_DIR}/examples")
endif()

if(${RUN_TEST})
    enable_testing()
    add_subdirectory("${CMAKE_SOURCE_DIR}/test")
endif()

if(${BUILD_DOC})
    add_subdirectory("${CMAKE_SOURCE_DIR}/doc")
endif()
