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
)


target_link_libraries(engine_test GTest::gtest_main SekaiEngine)
target_include_directories(engine_test PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)

add_test(NAME Test COMMAND engine_test)

include(GoogleTest)
gtest_discover_tests(engine_test)

set(BUILD_SHARED_LIBS ${PROJECT_BUILD_SHARED_LIBS})