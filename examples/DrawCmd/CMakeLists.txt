project(DrawCmd VERSION 1.0.0)

set(DrawCmd_SOURCES
    ./src/DrawCmd.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(DrawCmd ${DrawCmd_SOURCES})
target_link_libraries(DrawCmd PUBLIC SekaiEngine)
target_include_directories(DrawCmd PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(DrawCmd PRIVATE HAS_ENTRY_POINT)

if(MSVC)
    target_compile_options(DrawCmd PRIVATE /utf-8)
endif()

configure_platform_and_web(DrawCmd "${PROJECT_SOURCE_DIR}/assets")
