project(SoundMusic VERSION 1.0.0)

set(SoundMusic_SOURCES
    ./src/SoundMusic.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(SoundMusic ${SoundMusic_SOURCES})
target_link_libraries(SoundMusic PUBLIC SekaiEngine)
target_include_directories(SoundMusic PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(SoundMusic PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(SoundMusic
    "${CMAKE_SOURCE_DIR}/examples/assets/Mission Completed.wav"
    "${CMAKE_SOURCE_DIR}/examples/assets/Stream Loops 2023-11-29.ogg"
    "${CMAKE_SOURCE_DIR}/examples/assets/NotoSansTC-VariableFont_wght.ttf")
