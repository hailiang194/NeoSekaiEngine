project(InputKey VERSION 1.0.0)

set(InputKey_SOURCES
    ./src/InputKey.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(InputKey ${InputKey_SOURCES})
target_link_libraries(InputKey PUBLIC SekaiEngine)
target_include_directories(InputKey PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(InputKey PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(InputKey "${PROJECT_SOURCE_DIR}/assets")