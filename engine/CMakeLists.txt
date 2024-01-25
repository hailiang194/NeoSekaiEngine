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
    ./include/SekaiEngine/Layer/Layer.h
    ./include/SekaiEngine/Layer/LayerStack.h
    ./include/SekaiEngine/Input.h
    ./include/SekaiEngine/Math/Vector.h
)

set(ENGINE_SOURCES
    ./src/wrapper/raylib/MainLoop.cpp
    ./src/wrapper/raylib/Window.cpp
    ./src/wrapper/raylib/Input.cpp
    ./src/SekaiEngine/Application.cpp
    ./src/SekaiEngine/Window.cpp
    ./src/SekaiEngine/Event/Event.cpp
    ./src/SekaiEngine/Event/WindowEvent.cpp
    ./src/SekaiEngine/Layer/Layer.cpp
    ./src/SekaiEngine/Layer/LayerStack.cpp
    ./src/SekaiEngine/Math/Vector.cpp
)

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