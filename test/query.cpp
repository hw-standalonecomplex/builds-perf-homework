#include <catch.hpp>
#include <homework.hpp>
#include <sstream>

TEST_CASE("test query")
{
    std::stringstream stream;
    stream << "-122,48,one\n";
    stream << "-121,49,two\n";
    stream << "-121.99,47.99,three\n";

    mapbox::geometry::point<double> query_pt(-122.0001, 48.0001);
    std::size_t limit = 2;
    std::vector<homework::place_type> results = homework::run_query(stream, query_pt, limit);
    REQUIRE(results.size() == limit);
    REQUIRE(results[0].name == std::string("one"));
    REQUIRE(results[1].name == std::string("three"));
}