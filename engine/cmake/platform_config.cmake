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
        if(EMSCRIPTEN)
            # Emit the game as an ES6 module (js + wasm + preloaded data), not an html shell.
            # CMake's Emscripten toolchain already defaults CMAKE_EXECUTABLE_SUFFIX to ".js",
            # so the output lands at <target>.js with <target>.wasm and <target>.data beside it.
            # Link-time settings belong to the link, not to the compile flags:
            # SHELL: keeps each setting attached to its flag: target_link_options
            # de-duplicates repeated option names, which would otherwise collapse
            # the four -s settings into one -s with four values.
            target_link_options(${target} PRIVATE
                "SHELL:-s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -sMODULARIZE=1 -sEXPORT_ES6=1")
            foreach(asset IN LISTS resolved_assets)
                get_filename_component(asset_name "${asset}" NAME)
                # The destination after @ must be explicit: a bare trailing @ (empty
                # destination) makes emcc emit a nameless "/" entry, and the runtime
                # then fails to mknod it. Assets land flat in the VFS root, as on Desktop.
                target_link_options(${target} PRIVATE
                    "SHELL:--preload-file=\"${asset}@/${asset_name}\"")
            endforeach()

            # Each game gets its own output folder (out/<target>/) on the Web platform so
            # the shared UI index.html never collides across targets.
            set_target_properties(${target} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}")

            if(BUILD_WEB_UI)
                set(WEB_UI_WORK_DIR "${CMAKE_BINARY_DIR}/web-ui/${target}")

                # Stage a disposable copy of the generic Vue UI; the assembly artifacts
                # (glue + copied module files) live only in this working copy, never in the tree.
                file(COPY "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../web-ui/"
                          DESTINATION "${WEB_UI_WORK_DIR}")

                # The UI imports ./wasm/game, which re-exports the game's ES6 module factory.
                file(WRITE "${WEB_UI_WORK_DIR}/src/wasm/game.ts"
                          "export { default } from './${target}.js';\n")

                # Build the UI after the module pair is linked: install deps (first run only),
                # vite build with a relative base, then merge dist/ into the game's output folder.
                file(GENERATE OUTPUT "${WEB_UI_WORK_DIR}/build-web-ui.cmake" CONTENT
"if(NOT EXISTS \"${WEB_UI_WORK_DIR}/node_modules\")
    execute_process(COMMAND ${NPM_EXECUTABLE} install
                    RESULT_VARIABLE _inst
                    WORKING_DIRECTORY \"${WEB_UI_WORK_DIR}\")
    if(NOT _inst EQUAL 0)
        message(FATAL_ERROR \"npm install failed: \${_inst}\")
    endif()
endif()
execute_process(COMMAND ${NPM_EXECUTABLE} run build -- --base=${WEB_UI_BASE}
                RESULT_VARIABLE _build
                WORKING_DIRECTORY \"${WEB_UI_WORK_DIR}\")
if(NOT _build EQUAL 0)
    message(FATAL_ERROR \"npm run build failed: \${_build}\")
endif()
file(COPY \"${WEB_UI_WORK_DIR}/dist/\" DESTINATION \"$<TARGET_FILE_DIR:${target}>/\")
")

                add_custom_command(TARGET ${target} POST_BUILD
                    # Feed the freshly-linked module pair (and data) to the UI working copy
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "$<TARGET_FILE_DIR:${target}>/${target}.js"
                        "${WEB_UI_WORK_DIR}/src/wasm/${target}.js"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "$<TARGET_FILE_DIR:${target}>/${target}.wasm"
                        "${WEB_UI_WORK_DIR}/src/wasm/${target}.wasm")
                if(resolved_assets)
                    add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "$<TARGET_FILE_DIR:${target}>/${target}.data"
                            "${WEB_UI_WORK_DIR}/public/${target}.data")
                endif()
                add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -P "${WEB_UI_WORK_DIR}/build-web-ui.cmake")
            endif()
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
