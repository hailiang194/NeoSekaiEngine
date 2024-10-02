project(Shapes VERSION 1.0.0)

set(Shapes_SOURCES
    ./src/Shapes.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(Shapes ${Shapes_SOURCES})
target_link_libraries(Shapes PUBLIC SekaiEngine)
target_include_directories(Shapes PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(Shapes PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(Shapes "${PROJECT_SOURCE_DIR}/assets")