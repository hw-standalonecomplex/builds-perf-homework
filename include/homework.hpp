#pragma once

#include <boost/algorithm/string.hpp>
#include <mapbox/cheap_ruler.hpp>
#include <mapbox/geometry/point.hpp>

namespace homework {

struct place_type
{
    double distance;
    std::string name;
};

struct CompareDistance
{
    bool operator()(place_type p0, place_type p1)
    {
        return p0.distance < p1.distance;
    }
};

template <typename StreamType>
inline std::vector<place_type> run_query(StreamType& stream,
                                         mapbox::geometry::point<double> const& query_pt,
                                         std::size_t limit)
{
    std::vector<place_type> places;
    std::string input_line;
    while (std::getline(stream, input_line))
    {
        if (!input_line.empty())
        {
            std::vector<std::string> parts;
            boost::split(parts, input_line, boost::is_any_of(","));
            place_type place;
            double p0 = std::stod(parts[0]);
            double p1 = std::stod(parts[1]);
            mapbox::geometry::point<double> place_pt(p0, p1);
            mapbox::cheap_ruler::CheapRuler ruler(query_pt.y, mapbox::cheap_ruler::CheapRuler::Meters);
            place.distance = ruler.distance(query_pt, place_pt);
            place.name = std::string(parts[2]);
            places.push_back(place);
        }
    }
    std::sort(places.begin(), places.end(), CompareDistance());
    places.resize(limit);
    return places;
}

} // namespace homework