cmake_minimum_required(VERSION 3.8)
project(hpp_skel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mason.cmake)

# Enable extra warnings to adhere to https://github.com/mapbox/cpp/issues/37
set(DESIRED_WARNINGS "-Wall -Wextra -Wconversion -Wunreachable-code -Wuninitialized -pedantic-errors -Wold-style-cast -Wno-error=unused-variable -Wshadow -Wfloat-equal -Weffc++")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(DESIRED_WARNINGS "${DESIRED_WARNINGS} -Wmost")
endif()

# Note: -D_GLIBCXX_USE_CXX11_ABI=0 is needed to support mason packages that are precompiled libs
# Currently we only depend on a header only library, but this will help avoid issues when more libs are added via mason
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0 ${DESIRED_WARNINGS}")

# mason_use is a mason function within the mason.cmake file and provides ready-to-go vars, like "STATIC_LIBS" and "INCLUDE_DIRS"
mason_use(catch VERSION 1.9.6 HEADER_ONLY)
include_directories(SYSTEM ${MASON_PACKAGE_catch_INCLUDE_DIRS})

mason_use(benchmark VERSION 1.2.0)
include_directories(SYSTEM ${MASON_PACKAGE_benchmark_INCLUDE_DIRS})

mason_use(geometry VERSION 0.9.2 HEADER_ONLY)
include_directories(SYSTEM ${MASON_PACKAGE_geometry_INCLUDE_DIRS})

mason_use(variant VERSION 1.1.4 HEADER_ONLY)
include_directories(SYSTEM ${MASON_PACKAGE_variant_INCLUDE_DIRS})

mason_use(boost VERSION 1.62.0 HEADER_ONLY)
include_directories(SYSTEM ${MASON_PACKAGE_boost_INCLUDE_DIRS})

mason_use(cheap-ruler VERSION 2.5.3 HEADER_ONLY)
include_directories(SYSTEM ${MASON_PACKAGE_cheap-ruler_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB TEST_SOURCES test/*.cpp)
add_executable(unit-tests ${TEST_SOURCES})

file(GLOB BIN_SOURCES bin/*.cpp)
add_executable(nearest-places ${BIN_SOURCES})

# libbenchmark.a supports threads and therefore needs pthread support
find_package(Threads REQUIRED)
file(GLOB BENCH_SOURCES bench/*.cpp)
add_executable(bench-tests ${BENCH_SOURCES})

# link benchmark static library to the bench-tests binary so the bench tests know where to find the benchmark impl code
target_link_libraries(bench-tests ${MASON_PACKAGE_benchmark_STATIC_LIBS} ${CMAKE_THREAD_LIBS_INIT})
