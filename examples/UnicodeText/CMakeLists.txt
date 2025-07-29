project(UnicodeText VERSION 1.0.0)

set(UnicodeText_SOURCES
    ./src/UnicodeText.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(UnicodeText ${UnicodeText_SOURCES})
target_link_libraries(UnicodeText PUBLIC SekaiEngine)
target_include_directories(UnicodeText PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(UnicodeText PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(UnicodeText "${PROJECT_SOURCE_DIR}/assets")