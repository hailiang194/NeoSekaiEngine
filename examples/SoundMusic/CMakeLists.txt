project(SoundMusic VERSION 1.0.0)

set(SoundMusic_SOURCES
    ./src/SoundMusic.cpp
)

include(${CMAKE_SOURCE_DIR}/engine/cmake/platform_config.cmake)

add_executable(SoundMusic ${SoundMusic_SOURCES})
target_link_libraries(SoundMusic PUBLIC SekaiEngine)
target_include_directories(SoundMusic PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
# The labels are recorded DrawCmds, so they need a font face in the text engine.
# The face is the DrawCmd example's asset rather than a second copy of a 12MB file,
# the same way the test target reaches it.
target_compile_definitions(SoundMusic PRIVATE
    SOUNDMUSIC_FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../DrawCmd/assets/NotoSansTC-VariableFont_wght.ttf")
target_compile_definitions(SoundMusic PRIVATE HAS_ENTRY_POINT)

configure_platform_and_web(SoundMusic "${PROJECT_SOURCE_DIR}/assets")