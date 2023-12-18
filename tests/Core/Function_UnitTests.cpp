#include <Core/Function.h>
#include <catch2/catch_test_macros.hpp>

namespace Edgeleap
{

namespace Test
{

int test_function_1()
{
    return 55;
}

TEST_CASE("Function can contain any callable", "Construction & operator() testing")
{
    SECTION("empty constructor, call operator should return default return type value")
    {
        Edgeleap::Function<int(void)> empty_constructor_function;
        REQUIRE(empty_constructor_function() == 0);
    }

    SECTION("Static Pre-defined function")
    {
        Edgeleap::Function<int(void)> static_function(test_function_1);
        REQUIRE(static_function() == 55);
    }

    SECTION("Lambda without capture group")
    {
    }

    SECTION("Lambda with capture group")
    {
    }
}

}

}
