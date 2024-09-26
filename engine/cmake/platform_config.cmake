# PlatformConfig.cmake

# Define a function to configure platform-specific and Web-specific settings
function(configure_platform_and_web target asset_path)
    # Apple-specific configuration
    if(APPLE)
        target_link_libraries(${target} PRIVATE "-framework IOKit"
                                                "-framework Cocoa"
                                                "-framework OpenGL")
    endif()
    # Web-specific configuration
    if(${PLATFORM} STREQUAL "Web" OR EMSCRIPTEN)
        set_target_properties(${target} PROPERTIES SUFFIX ".html")

        if(EMSCRIPTEN)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file ${asset_path}@" PARENT_SCOPE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY  --preload-file ${asset_path}@" PARENT_SCOPE)
            set(CMAKE_EXECUTABLE_SUFFIX ".html" PARENT_SCOPE)
        endif()
    else()
        add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${asset_path}"
            $<TARGET_FILE_DIR:${target}>)
    endif()
    
endfunction()