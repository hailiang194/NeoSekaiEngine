if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ")
endif()
set(DOXYGEN_INPUT_FILES
    ${CMAKE_SOURCE_DIR}/README.md 
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/doc/pages
)

find_package(Doxygen REQUIRED dot)
if(${DOXYGEN_FOUND})
    message(STATUS "Doxygen founded, version=${DOXYGEN_VERSION}")
    set(DOXYGEN_GENERATE_HTML TRUE)
    set(DOXYGEN_DOTFILE_DIRS ${CMAKE_SOURCE_DIR}/doc/diagrams)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    set(DOXYGEN_PROJECT_NAME "NeoSekaiEngine")
    doxygen_add_docs(EngineDoc ${DOXYGEN_INPUT_FILES} COMMENT "Generate NeoSekaiEngine document" )
    
endif()