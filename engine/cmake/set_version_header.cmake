string(TIMESTAMP RELEASE_DATE "%Y-%m-%d")
message(STATUS "Configure version")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/version.h.cmake ${CMAKE_BINARY_DIR}/config/version.h)