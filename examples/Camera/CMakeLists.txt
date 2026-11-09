project(Camera VERSION 1.0.0)

set(Camera_SOURCES
    ./src/Camera.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(Camera ${Camera_SOURCES})
target_link_libraries(Camera PUBLIC SekaiEngine)
target_include_directories(Camera PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(Camera PRIVATE HAS_ENTRY_POINT)

if(MSVC)
    target_compile_options(Camera PRIVATE /utf-8)
endif()

configure_platform_and_web(Camera
    "${CMAKE_SOURCE_DIR}/examples/assets/NotoSansTC-VariableFont_wght.ttf")
