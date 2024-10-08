string(TIMESTAMP RELEASE_DATE "%Y-%m-%d")
foreach(HEADER_FILE ${ENGINE_PUBLIC_HEADERS})
    string(FIND ${HEADER_FILE} "${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine" POS)
    if(${POS} EQUAL "-1")
        continue()
    endif()
    # message(STATUS ${HEADER_FILE})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine" "SekaiEngine" HEADER_NAME ${HEADER_FILE})
    set(INCLUDE_PART "${INCLUDE_PART}#include \"${HEADER_NAME}\"\n")
endforeach()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/SekaiEngine.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/include/SekaiEngine.h)
