#include <catch.hpp>
#include <homework.hpp>
#include <sstream>

TEST_CASE("test comparing places")
{
    homework::place_type p0
        p0.distance = 0.0;
    p0.name = "closest";

    homework::place_type p1;
    p1.distance = 1.0;
    p1.name = "farther";

    homework::CompareDistance compare;
    REQUIRE(compare(p0, p1));
}