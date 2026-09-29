# PlatformConfig.cmake

# Define a function to configure platform-specific and Web-specific settings
#
# configure_platform_and_web(<target> [<asset>...])
#
# Each <asset> entry is either a file or a directory. Directories are expanded
# to the files they contain; files are taken as-is. Assets are delivered flat,
# under their own file name, into the target's build folder on Desktop and into
# the emscripten filesystem root of the .data payload on Web.
function(configure_platform_and_web target)
    set(resolved_assets)

    foreach(entry IN LISTS ARGN)
        if(IS_DIRECTORY "${entry}")
            file(GLOB_RECURSE contained CONFIGURE_DEPENDS "${entry}/*")
            list(APPEND resolved_assets ${contained})
        elseif(EXISTS "${entry}")
            list(APPEND resolved_assets "${entry}")
        else()
            message(FATAL_ERROR
                "configure_platform_and_web: asset '${entry}' declared by target "
                "'${target}' is neither an existing file nor an existing directory")
        endif()
    endforeach()

    # Assets are delivered by file name only, so two entries sharing a name would
    # overwrite each other in the build folder and collide inside one .data.
    set(seen_names)
    set(seen_assets)
    foreach(asset IN LISTS resolved_assets)
        get_filename_component(asset_name "${asset}" NAME)
        if(asset_name IN_LIST seen_names)
            list(FIND seen_names "${asset_name}" seen_index)
            list(GET seen_assets ${seen_index} first_seen)
            message(WARNING
                "configure_platform_and_web: target '${target}' declares "
                "'${first_seen}' and '${asset}', which share the file name "
                "'${asset_name}' and will overwrite each other.")
        endif()
        list(APPEND seen_names "${asset_name}")
        list(APPEND seen_assets "${asset}")
    endforeach()

    # Apple-specific configuration
    if(APPLE)
        target_link_libraries(${target} PRIVATE "-framework IOKit"
                                                "-framework Cocoa"
                                                "-framework OpenGL")
    endif()
    # Web-specific configuration
    if("${PLATFORM}" STREQUAL "Web" OR EMSCRIPTEN)
        set_target_properties(${target} PROPERTIES SUFFIX ".html")

        if(EMSCRIPTEN)
            # Link-time settings belong to the link, not to the compile flags.
            # SHELL: keeps each setting attached to its flag: target_link_options
            # de-duplicates repeated option names, which would otherwise collapse
            # the four -s settings into one -s with four values.
            target_link_options(${target} PRIVATE
                "SHELL:-s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
            foreach(asset IN LISTS resolved_assets)
                target_link_options(${target} PRIVATE
                    "SHELL:--preload-file \"${asset}@\"")
            endforeach()
            set(CMAKE_EXECUTABLE_SUFFIX ".html" PARENT_SCOPE)
        endif()
    else()
        foreach(asset IN LISTS resolved_assets)
            get_filename_component(asset_name "${asset}" NAME)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${asset}"
                    "$<TARGET_FILE_DIR:${target}>/${asset_name}"
                VERBATIM)
        endforeach()
    endif()

endfunction()
