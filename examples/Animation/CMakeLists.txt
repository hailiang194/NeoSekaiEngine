project(Animation VERSION 1.0.0)

set(Animation_SOURCES
    ./src/Animation.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(Animation ${Animation_SOURCES})
target_link_libraries(Animation PUBLIC SekaiEngine)
target_include_directories(Animation PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(Animation PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(Animation
    "${CMAKE_SOURCE_DIR}/examples/assets/NotoSansTC-VariableFont_wght.ttf")
