project(SekaiEngine VERSION 1.0.0)

set(ENGINE_PUBLIC_HEADER
    ./include/wrapper/raylib/Window.h
    ./include/SekaiEngine.h
    ./include/SekaiEngine/EntryPoint.h
    ./include/SekaiEngine/BaseType.h
    ./include/SekaiEngine/Application.h
    ./include/SekaiEngine/Window.h
    ./include/SekaiEngine/Event/Event.h
    ./include/SekaiEngine/Event/WindowEvent.h
)

set(ENGINE_SOURCES
    ./src/wrapper/raylib/Window.cpp
    ./src/SekaiEngine/Application.cpp
    ./src/SekaiEngine/Window.cpp
    ./src/SekaiEngine/Event/Event.cpp
    ./src/SekaiEngine/Event/WindowEvent.cpp
)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework OpenGL")
endif()

# Web Configuration
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

option(BUILD_SHARED_LIBS "Build shared lib" ON)

add_subdirectory(./vendors/raylib)

add_library(SekaiEngine ${ENGINE_PUBLIC_HEADER} ${ENGINE_SOURCES})
target_link_libraries(SekaiEngine PRIVATE raylib)
target_compile_definitions(SekaiEngine PRIVATE USE_RAYLIB)

if(BUILD_SHARED_LIBS)
    message(STATUS "Build shared lib for SekaiEngine")
    if (WIN32)
        target_compile_definitions(SekaiEngine
                                   PRIVATE $<BUILD_INTERFACE:BUILD_LIBTYPE_SHARED>
                                   INTERFACE $<INSTALL_INTERFACE:USE_LIBTYPE_SHARED>
                                   )
    endif ()
    set_property(TARGET SekaiEngine PROPERTY POSITION_INDEPENDENT_CODE ON)
else()
    message(STATUS "Build static lib for SekaiEngine")
endif()

target_include_directories(SekaiEngine
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>

)