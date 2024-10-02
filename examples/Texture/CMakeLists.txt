project(Texture VERSION 1.0.0)

set(Texture_SOURCES
    ./src/Texture.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(Texture ${Texture_SOURCES})
target_link_libraries(Texture PUBLIC SekaiEngine)
target_include_directories(Texture PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(Texture PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(Texture "${PROJECT_SOURCE_DIR}/assets")