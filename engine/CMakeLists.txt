project(SekaiEngine VERSION 0.4.0)

option(BUILD_WEB_UI "Build the Vue web UI for web targets and stage it into the game output dir" OFF)
set(WEB_UI_BASE "./" CACHE STRING "Base URL for the built web UI (vite --base)")

find_program(NPM_EXECUTABLE npm)
if(BUILD_WEB_UI AND EMSCRIPTEN AND NOT NPM_EXECUTABLE)
    message(FATAL_ERROR "BUILD_WEB_UI=ON requires npm (the Vue UI is built with vite); install Node.js/npm or add it to PATH")
endif()

include(cmake/import_engine.cmake)
include(cmake/set_version_header.cmake)

set(WRAPPER_RAYLIB_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Font.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Log.h
)

set(WRAPPER_RAYLIB_SOURCES
    ./src/wrapper/raylib/MainLoop.cpp
    ./src/wrapper/raylib/Window.cpp
    ./src/wrapper/raylib/Input.cpp
    ./src/wrapper/raylib/RendererAPI.cpp
    ./src/wrapper/raylib/Parser.cpp
    ./src/wrapper/raylib/Camera.cpp
    ./src/wrapper/raylib/Texture.cpp
    ./src/wrapper/raylib/Timer.cpp
    ./src/wrapper/raylib/Font.cpp
    ./src/wrapper/raylib/TextEngine.cpp
    ./src/wrapper/raylib/AudioDevice.cpp
    ./src/wrapper/raylib/Sound.cpp
    ./src/wrapper/raylib/MusicStream.cpp
    ./src/wrapper/raylib/Log.cpp
)

set(ENGINE_PUBLIC_HEADERS
    ${WRAPPER_RAYLIB_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/BaseType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Event/Event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Event/WindowEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Event/ApplicationEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Layer/Layer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Layer/LayerStack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Input.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Math/Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/RendererAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Color.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/RenderCommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Camera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Timer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Shape/Shape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Shape/Circle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Shape/Rectangle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/RenderProperties.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/DrawCmd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Math/Utility.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Font.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Math/Collision.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/EntryPoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/UI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/TextureUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/CircleUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/RectangleUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/GroupUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Audio/Device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Audio/Sound.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Audio/MusicStream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/TextEngine/TextEngine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Log.h
)

find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_custom_target(check_public_header_exports ALL
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/lint_exports.py ${ENGINE_PUBLIC_HEADERS}
        COMMENT "Checking public headers for missing EXTENDAPI"
        VERBATIM
    )
else()
    message(WARNING "Python3 interpreter not found: the public header EXTENDAPI check will not run.")
endif()

include(cmake/set_include_all_header.cmake)

set(ENGINE_SOURCES
    ${WRAPPER_RAYLIB_SOURCES}
    ./src/SekaiEngine/Application.cpp
    ./src/SekaiEngine/Window.cpp
    ./src/SekaiEngine/Event/Event.cpp
    ./src/SekaiEngine/Event/WindowEvent.cpp
    ./src/SekaiEngine/Event/ApplicationEvent.cpp
    ./src/SekaiEngine/Layer/Layer.cpp
    ./src/SekaiEngine/Layer/LayerStack.cpp
    ./src/SekaiEngine/Math/Vector.cpp
    ./src/SekaiEngine/Render/Color.cpp
    ./src/SekaiEngine/Render/Renderer.cpp
    ./src/SekaiEngine/Render/Camera.cpp
    ./src/SekaiEngine/Timer.cpp
    ./src/SekaiEngine/Shape/Circle.cpp
    ./src/SekaiEngine/Shape/Rectangle.cpp
    ./src/SekaiEngine/Render/RenderCommand.cpp
    ./src/SekaiEngine/Render/RenderProperties.cpp
    ./src/SekaiEngine/Render/DrawCmd.cpp
    ./src/SekaiEngine/Math/Utility.cpp
    ./src/SekaiEngine/Render/Font.cpp
    ./src/SekaiEngine/Math/Collision.cpp
    ./src/SekaiEngine/Object/UI.cpp
    ./src/SekaiEngine/Object/TextureUI.cpp
    ./src/SekaiEngine/Object/CircleUI.cpp
    ./src/SekaiEngine/Object/RectangleUI.cpp
    ./src/SekaiEngine/Object/GroupUI.cpp
    ./src/SekaiEngine/Audio/Sound.cpp
    ./src/SekaiEngine/Audio/MusicStream.cpp
    ./src/SekaiEngine/TextEngine/TextEngine.cpp
    ./src/SekaiEngine/Log.cpp
    ./src/SekaiEngine/LogCrash.cpp
)

option(BUILD_SHARED_LIBS "Build shared lib" ON)

add_subdirectory(./vendors/raylib)
add_subdirectory(./vendors/freetype)

# The engine starts a std::thread in Application::Run on desktop, and consumers
# (the examples and engine_test) start threads of their own, so the -pthread
# compile and link flags have to reach them through SekaiEngine's interface.
find_package(Threads REQUIRED)

add_library(SekaiEngine ${ENGINE_PUBLIC_HEADERS} ${ENGINE_SOURCES})
# A PUBLIC compile feature rather than a directory-scope standard variable: a
# variable does not leave engine/, so the examples, engine_test and downstream
# projects would sit at their toolchain's default instead of the library's.
target_compile_features(SekaiEngine PUBLIC cxx_std_17)
target_link_libraries(SekaiEngine PUBLIC Threads::Threads)
target_link_libraries(SekaiEngine PRIVATE raylib freetype)
target_include_directories(SekaiEngine PUBLIC ./vendors/freetype/include)
target_compile_definitions(SekaiEngine PRIVATE USE_RAYLIB)

# Logging: vendored plog drives the desktop sinks; its char type must stay
# UTF-8 on Windows. The internal ring/crash module lives under src/.
target_include_directories(SekaiEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendors/plog/include)
target_include_directories(SekaiEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(SekaiEngine PRIVATE PLOG_CHAR_IS_UTF8=1)

if(${PLATFORM} STREQUAL "Web" OR EMSCRIPTEN)
    target_compile_definitions(SekaiEngine PRIVATE PLATFORM_WEB)
endif()

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

target_include_directories(SekaiEngine
    PUBLIC ${CMAKE_BINARY_DIR}/config/
)
