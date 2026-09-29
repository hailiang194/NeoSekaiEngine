project(test)

set(PROJECT_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF)

# include(FetchContent)
# FetchContent_Declare(
# googletest
# URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(vendors/googletest)
# FetchContent_MakeAvailable(googletest)


add_executable(engine_test
    src/vector.cpp
    src/color.cpp
    src/collision.cpp
    src/timestep.cpp
    src/text_engine.cpp
    src/frame_threading.cpp
)


target_link_libraries(engine_test GTest::gtest_main SekaiEngine)
target_include_directories(engine_test PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(engine_test PRIVATE
    FONT_PATH_NOTO="${CMAKE_CURRENT_SOURCE_DIR}/../examples/DrawCmd/assets/NotoSansTC-VariableFont_wght.ttf"
)

add_test(NAME Test COMMAND engine_test)

include(GoogleTest)
gtest_discover_tests(engine_test)

# Logging is verified by a framework-free self-check (no Googletest): exit
# code 0 means every check passed (see openspec/changes/engine-logging/design.md D10).
add_executable(log_selftest
    src/log_selftest.cpp
)

target_link_libraries(log_selftest SekaiEngine)
target_include_directories(log_selftest PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/engine/src
    ${CMAKE_SOURCE_DIR}/engine/vendors/plog/include
)
target_compile_definitions(log_selftest PRIVATE PLOG_CHAR_IS_UTF8=1)

add_test(NAME LogSelfCheck COMMAND log_selftest)

set(BUILD_SHARED_LIBS ${PROJECT_BUILD_SHARED_LIBS})