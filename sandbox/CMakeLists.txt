project(Sandbox VERSION 1.0.0)

set(SANDBOX_SOURCES
    ./src/Sandbox.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})
target_link_libraries(Sandbox PUBLIC SekaiEngine)
target_include_directories(Sandbox PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)
target_compile_definitions(Sandbox PRIVATE HAS_ENTRY_POINT)