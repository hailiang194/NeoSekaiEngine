project(SekaiEngine VERSION 0.2.0)
set (CMAKE_CXX_STANDARD 11)

include(cmake/import_engine.cmake)
include(cmake/set_version_header.cmake)

set(WRAPPER_RAYLIB_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/wrapper/raylib/Font.h
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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Params/RenderParams.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Params/TextureRenderParams.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Params/RectangleRenderParams.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Params/CircleRenderParams.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Math/Utility.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Render/Font.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Math/Collision.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/EntryPoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/UI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/TextureUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/CircleUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/RectangleUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine/Object/GroupUI.h
)

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
    ./src/SekaiEngine/Render/Texture.cpp
    ./src/SekaiEngine/Shape/Shape.cpp
    ./src/SekaiEngine/Shape/Circle.cpp
    ./src/SekaiEngine/Shape/Rectangle.cpp
    ./src/SekaiEngine/Render/RenderCommand.cpp
    ./src/SekaiEngine/Render/RenderProperties.cpp
    ./src/SekaiEngine/Render/Params/RenderParams.cpp
    ./src/SekaiEngine/Render/Params/TextureRenderParams.cpp
    ./src/SekaiEngine/Render/Params/RectangleRenderParams.cpp
    ./src/SekaiEngine/Render/Params/CircleRenderParams.cpp
    ./src/SekaiEngine/Math/Utility.cpp
    ./src/SekaiEngine/Render/Font.cpp
    ./src/SekaiEngine/Math/Collision.cpp
    ./src/SekaiEngine/Object/UI.cpp
    ./src/SekaiEngine/Object/TextureUI.cpp
    ./src/SekaiEngine/Object/CircleUI.cpp
    ./src/SekaiEngine/Object/RectangleUI.cpp
    ./src/SekaiEngine/Object/GroupUI.cpp
)

option(BUILD_SHARED_LIBS "Build shared lib" ON)

add_subdirectory(./vendors/raylib)

add_library(SekaiEngine ${ENGINE_PUBLIC_HEADERS} ${ENGINE_SOURCES})
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

target_include_directories(SekaiEngine
    PUBLIC ${CMAKE_BINARY_DIR}/config/
)