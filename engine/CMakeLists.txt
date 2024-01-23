project(SekaiEngine VERSION 1.0.0)

set(ENGINE_PUBLIC_HEADER
    ./include/BaseType.h
    ./include/Test.h
)

set(ENGINE_SOURCES
    ./src/Test.cpp
)


option(BUILD_SHARED_LIBS "Build shared lib" ON)

add_library(SekaiEngine ${ENGINE_PUBLIC_HEADER} ${ENGINE_SOURCES})


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