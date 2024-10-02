project(InputMouse VERSION 1.0.0)

set(InputMouse_SOURCES
    ./src/InputMouse.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(InputMouse ${InputMouse_SOURCES})
target_link_libraries(InputMouse PUBLIC SekaiEngine)
target_include_directories(InputMouse PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(InputMouse PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(InputMouse "${PROJECT_SOURCE_DIR}/assets")