cmake_minimum_required(VERSION 3.9)

file(GLOB_RECURSE GAEA_SOURCES
    ./**.cpp)
file(GLOB_RECURSE GAEA_INCLUDES
    ./**.h)

add_library(Gaea STATIC ${GAEA_SOURCES})
set_target_properties(Gaea PROPERTIES PUBLIC_HEADERS .)
target_include_directories(Gaea
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

