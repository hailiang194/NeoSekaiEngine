project(Touch VERSION 1.0.0)

set(Touch_SOURCES
    ./src/Touch.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(Touch ${Touch_SOURCES})
target_link_libraries(Touch PUBLIC SekaiEngine)
target_include_directories(Touch PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(Touch PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(Touch "${PROJECT_SOURCE_DIR}/assets")