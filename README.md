## Background

The Core Tech team develops many single-purpose C++ libraries and brings them together into larger applications using the C++ packaging tool called [mason](https://github.com/mapbox/mason/).

We are looking for someone who can help us scale our development by bringing:

 - A high level of attention to detail for code quality and performance
 - A desire to notice things that might go wrong before they do
 - A an ability to assemble a holistic view the system, all the way from code and build systems to packaging and production benchmarking
 - A penchant for diving deep into all the interesting - expected and unexpected - problems of low level code that arise along the way
 - A talent for writing and communicating their work so the team can follow along and learn

If you join the team we'll be looking for you to:

 - Apply (or ramp up) your C++ knowledge to be able to review and improve our C++ development
 - Take the lead, with mentorship support from Dane Springmeyer, on maintaining and developing the future of `mason`


## A representative problem

Today we'd like to engage you, in a hands on way, with a small codebase that reflects recent challenges we've faced in writing and building robust and high performance C++ code.

The code is meant to mirror what a programmer, new to C++, might write at Mapbox in that:

 - The project layout is based on the Core Tech team's https://github.com/mapbox/hpp-skel conventions such that it contains:
   - benchmarks
   - unit tests
   - modular code spread across the `include/`, `./test`, and `./bin` directories
   - a `Makefile` wrapper around a cmake build system
 - It uses modern C++14 features and various libraries from mason as dependencies
 - It deals with parsing and querying geographical data extracted from OpenStreetMap
 - It contains a variety of mistakes from human error
 - It contains potential for performance optimization


## Your exercise

We'd like you to dig into the build system and code as if:

- The developer who wrote the code is on an extended vacation
- You've been ask to take full responsibility for the project
- Before packaging in mason, you've been ask to review and fix the code

We recommend you:

- Build the code on either Linux or OS X (it is not intended to work on windows)
- Fix any [compile](https://github.com/mapbox/cpp/blob/master/glossary.md#compiler) or [linking](https://github.com/mapbox/cpp/blob/master/glossary.md#linking) errors you encounter
- Ensure the tests and benchmarks run
- Dive into the code in `./include/` and `./bin` to understand what it is doing:
  - Parsing a CSV file of locations (`./data/places.txt`)
  - Calculating the distance from a hard-coded query point to each place in the input data
  - Finding the top 10 closest locations in the input data to the query point
  - Printing those 10 locations to the console.
- Review the code and build system to make sure:
  - you find any problems related to compiling, linking, robustness, or security
  - you identify and apply any obvious [optimizations](https://github.com/mapbox/cpp/blob/master/glossary.md#optimization-technique)
- Use the `time ./cmake-build/nearest-places ./data/places.txt` command or `make bench` to evaluate performance changes with your code changes.

We'd like you to provide us with two products:

1) A writeup of:

- a) how you built the code
- b) what problems you identified along the way in building or running the code and what you did (or would do) to try to solve them
- c) reasoning behind any performance or efficient changes you see (or made).

2) A set of changes, in the form of a diff, to the code or build system you made while working. Create this by running `git diff`.

Write as if your audience is the original developer of the code and you want to help them learn - when they return from vacation - what you learned in improving their code.

Please plan to spend around 2-3 hours digging into this problem and writing up your report. We are not looking for perfection or any particular result. Rather we want to learn how you approach problem solving, how you communicate what you understand and what you don't, and how comfortable you are in adapting to the modular way that we structure C++ code.


## A guide to the code

This project includes a mostly working C++ project.

Building depends on:

 - Either clang++ >= 4.x or g++ >= 4.9
 - C++14 support (on ubuntu that means `libstdc++` development headers >= 4.9 are installed, like `apt-get install libstdc++-4.9-dev`)
 - curl command line tool
 - make
 - cmake
 - A http connection to allow mason to download packages (called by cmake)

You can build with:

 - make release

You can run the unit tests with:

 - make test

You can run benchmarks with:

 - make bench

You can clean the build with:

 - make clean

You can clean the dependencies with:

 - make distclean

The API surface is a command line tool called `nearest-places` that can be run (after the code is built) like:


```
$ ./cmake-build/nearest-places ./data/places.txt
```

Which should dump:

```
reading ./data/places.txt
Top nearest 10 locations:
Sanitary Market Building
First & Pine Building
So Much Yarn
Shug's Soda Fountain & Ice Cream
Old Stove Brewing Co.
First & Pine Building
Pike Place Chowder
Bayou on 1st
Farvahar Cafe
Jasmine
```

The `./data/places.txt`, for reference was created like:


```bash
wget http://download.geofabrik.de/north-america/us/washington-latest.osm.pbf
mason install osmium-tool 1.7.1
mason link osmium-tool 1.7.1
# convert OSM data to ldjson with http://docs.osmcode.org/osmium/latest/osmium-export.html
./mason_packages/.link/bin/osmium export washington-latest.osm.pbf -o washington-latest.json -f geojsonseq -c export-config.json
# convert to simplified csv for the downstream example project to use (avoids needing to parse geojson)
node scripts/geojson2csv.js washington-latest.json > ./data/places.txt
```

