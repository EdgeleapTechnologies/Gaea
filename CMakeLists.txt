cmake_minimum_required(VERSION 3.9)

include(CMake/Warnings.cmake)

# ============================ Functions =========================== #
function(extract_platform_dependent_sources
        all_sources # input
        platform_independent_sources # output
        windows_sources #output
        linux_sources #output
        macos_sources #output
        posix_sources #output
)
    set(platform_independent_result)
    set(windows_result)
    set(linux_result)
    set(macos_result)
    set(posix_result)

    foreach(source ${all_sources})
        if(source MATCHES ".*\/[W|w]indows\/.*") #windows source file
            list(APPEND windows_result ${source})
        elseif(source MATCHES ".*\/[L|l]inux\/.*") #linux source file
            list(APPEND linux_result ${source})
        elseif(source MATCHES ".*\/[M|m][A|a][C|c][O|o][S|s]\/.*") #macos source file
            list(APPEND macos_result ${source})
        elseif(source MATCHES ".*\/[P|p][O|o][S|s][I|i][X|x]\/.*") #posix source file
            list(APPEND posix_result ${source})
        else()
            list(APPEND platform_independent_result ${source})
        endif()
    endforeach()

    set(${platform_independent_sources} ${platform_independent_result} PARENT_SCOPE)
    set(${windows_sources} ${windows_result} PARENT_SCOPE)
    set(${linux_sources} ${linux_result} PARENT_SCOPE)
    set(${macos_sources} ${macos_result} PARENT_SCOPE)
    set(${posix_sources} ${posix_result} PARENT_SCOPE)
endfunction()
# ================================================================== #

project(Gaea LANGUAGES C CXX)

set(GAEA_LIB "Gaea")
file(GLOB_RECURSE GAEA_SOURCES
    ./**.cpp)
extract_platform_dependent_sources("${GAEA_SOURCES}"
    gaea_common_sources
    gaea_windows_sources
    gaea_linux_sources
    gaea_macos_sources
    gaea_posix_sources)

add_library(${GAEA_LIB} STATIC ${gaea_common_sources})
configure_compiler_warnings("${GAEA_LIB}")

set_property(TARGET ${GAEA_LIB} PROPERTY CXX_STANDARD 20)
set_target_properties(${GAEA_LIB} PROPERTIES PUBLIC_HEADERS .)
target_include_directories(${GAEA_LIB}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ==================== Platform Specific Sources ========================== #
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_sources(${GAEA_LIB} PRIVATE ${gaea_linux_sources} ${gaea_posix_sources})
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    target_sources(${GAEA_LIB} PRIVATE ${gaea_macos_sources} ${gaea_posix_sources})
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_sources(${GAEA_LIB} PRIVATE ${gaea_windows_sources})
endif()
# ========================================================================= #
