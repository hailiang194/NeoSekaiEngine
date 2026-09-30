if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ")
endif()
# The paths below are relative to the source root, which is also the working
# directory the doc build runs in. Absolute paths would end up baked into the
# generated page titles and output file names, so a checkout in /home/alice
# would ship a doc that reads /home/alice all over it.
set(DOXYGEN_INPUT_FILES
    README.md 
    THIRD_PARTY_NOTICES.md
    engine/include
    engine/src
    doc/pages
)

find_package(Doxygen REQUIRED dot)
if(${DOXYGEN_FOUND})
    message(STATUS "Doxygen founded, version=${DOXYGEN_VERSION}")
    set(DOXYGEN_GENERATE_HTML TRUE)
    set(DOXYGEN_DOTFILE_DIRS doc/diagrams)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_PROJECT_NAME "NeoSekaiEngine")
    # The raylib wrapper headers wrap their whole body in #ifdef USE_RAYLIB, so
    # without this the preprocessor strips every declaration out of them.
    set(DOXYGEN_PREDEFINED "USE_RAYLIB")
    # Only the headers and the pages: the .cpp files hold no public API, and
    # parsing them would only report every private helper as undocumented.
    set(DOXYGEN_FILE_PATTERNS "*.md" "*.h" "*.dox")
    # Pulled in by the \include commands on the examples page.
    set(DOXYGEN_EXAMPLE_PATH examples)
    doxygen_add_docs(EngineDoc ${DOXYGEN_INPUT_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generate NeoSekaiEngine document" )
    
endif()
