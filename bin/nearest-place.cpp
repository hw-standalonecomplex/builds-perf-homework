#include <fstream>
#include <homework.hpp>
#include <iostream>
#include <mapbox/geometry/point.hpp>
#include <vector>

void print_usage(const char* prgname) {
    std::cerr << "Usage: " << prgname << " places.txt\n";
    std::exit(0);
}

int main(int argc, char* argv[]) {
    if (argc == 1 || argc > 2) {
        print_usage(argv[0]);
    }

    std::string filename = argv[1];

    try {

        std::ifstream stream(filename);
        if (!stream.is_open())
        {
            throw std::runtime_error("could not open: '" + filename + "'");
        }

        std::clog << "reading " << filename << "\n";

        // Use Pike Market in Seattle for our query point
        mapbox::geometry::point<double> query_pt(-122.341,47.60941);
        std::size_t limit = 10;
        std::vector<homework::place_type> results = homework::run_query(stream,query_pt,limit);
        std::clog << "Top nearest " << limit  << " locations:\n";
        for (auto const& place_type : results) {
            std::clog << place_type.name << "\n";
        }
        return 0;
    } catch (std::exception const& ex) {
        std::clog << "error: " << ex.what() << "\n";
        return -1;        
    }
}