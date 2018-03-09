#include <benchmark/benchmark.h>
#include <homework.hpp>
#include <sstream>
#include <stdexcept>

static void nearest_places(benchmark::State& state) // NOLINT google-runtime-references
{
    std::string filename("/data/places.txt");
    std::ifstream stream(filename);

    std::size_t limit = 10;
    mapbox::geometry::point<double> query_pt(-122.341, 47.60941);

    while (state.KeepRunning())
    {
        stream.seekg(0, std::ios::beg);
        std::vector<homework::place_type> results = homework::run_query(stream, query_pt, limit);
        benchmark::DoNotOptimize(&results[0]);
        benchmark::ClobberMemory();
    }
}

int main(int argc, char* argv[])
{
    benchmark::RegisterBenchmark("nearest_places", nearest_places)->Threads(2)->Threads(4)->Threads(8); // NOLINT clang-analyzer-cplusplus.NewDeleteLeaks
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}