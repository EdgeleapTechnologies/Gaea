file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        ./**.cpp
        ./**.hpp
        ./**.h
        )

#gaea tests are not contained in the default build and need its target
#needs to be explicitly built
add_executable(gaea-tests EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_include_directories(gaea-tests PRIVATE ../ThirdParty/)
target_link_libraries(gaea-tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(gaea-tests PRIVATE Gaea)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(gaea-tests)
