project(BasicWindow VERSION 1.0.0)

set(BasicWindow_SOURCES
    ./src/BasicWindow.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(BasicWindow ${BasicWindow_SOURCES})
target_link_libraries(BasicWindow PUBLIC SekaiEngine)
target_include_directories(BasicWindow PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(BasicWindow PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(BasicWindow "${PROJECT_SOURCE_DIR}/assets")