## Background

The Core Tech team develops many single-purpose C++ libraries and brings them together into larger applications using the C++ packaging tool called [mason](https://github.com/mapbox/mason/).

We are looking for someone who can help us scale our development by bringing:

 - A high level of attention to detail for code quality and performance
 - A desire to notice things that might go wrong before they do
 - A will to struggle past seemingly obscure compile and linker errors
 - A penchant for diving deep into all the interesting - expected and unexpected - problems of low level code that arise along the way
 - An ability to assemble a holistic view the system, all the way from code and build systems to packaging and production benchmarking
 - A talent for writing and communicating their work so the team can follow along and learn

If you join the team we'll be looking for you to:

 - Apply (or significantly ramp up) your C++ knowledge to be able to review and improve our C++ development
 - Take the lead, with mentorship support from Dane Springmeyer, on maintaining and developing the future of `mason`


## A representative problem

Today we'd like to engage you, in a hands on way, with a small codebase that reflects recent challenges we've faced in writing and building robust and high performance C++ code.

The code is meant to reflect what a programmer, new to C++, might write at Mapbox in that:

 - The project layout is based on the Core Tech team's https://github.com/mapbox/hpp-skel conventions such that it contains:
   - benchmarks
   - unit tests
   - modular code spread across the `include/`, `./test`, and `./bin` directories
   - a `Makefile` wrapper around a cmake build system
 - It uses modern C++14 features and various libraries from mason as dependencies
 - It deals with parsing and querying geographical data extracted from OpenStreetMap
 - It contains a variety of mistakes from human error
 - It contains significant (but potentially non-obvious) potential for performance optimization


## Your exercise

We'd like you to dig into the sample project (code + builds system) as if:

- The developer who wrote the code is on an extended vacation
- You've been ask to take full responsibility for the project
- After your work is done the command line program inside the code, called `nearest-places` will be:
  - packaged in mason
  - deployed to production
  - sent input data that is > 1 GB of data (so even larger than the sample data included below)

Please plan to spend around 2-3 hours on this exercise. We are not looking for perfection or any particular result. Rather we want to learn:

 - how you approach problem solving
 - how you communicate what you understand and what you don't
 - how comfortable you are in adapting to the modular way that we structure C++ code.

So, to recap, your exercise is to dig into the code and build system in this repo with the knowledge it will soon go into production. Since you won't have time to fix everything possible, we want you to focus on what you can fix, and write about what more you'd do if you have more time.

Specifically, we recommend you:

- Build the code on either Linux or OS X
  - If you are on Windows please get in touch and we can help you get set up to run the code in Docker
- Fix any [compile](https://github.com/mapbox/cpp/blob/master/glossary.md#compiler) or [linking](https://github.com/mapbox/cpp/blob/master/glossary.md#linking) errors you encounter
- Ensure the tests and benchmarks run correctly
- Dive into the code in `./include/` and `./bin` to understand what it is doing:
  - Parsing a CSV file of locations (`./data/places.txt`)
  - Calculating the distance from a hard-coded query point to each place in the input data
  - Finding the top 10 closest locations in the input data to the query point
  - Printing those 10 locations to the console.
- Review the code and build system to:
  - identify problems related to compiling, linking, robustness, or security
  - identify code [optimizations](https://github.com/mapbox/cpp/blob/master/glossary.md#optimization-technique)
- Use the `time ./cmake-build/nearest-places ./data/places.txt` command or `make bench` to evaluate performance impacts of any changes you make to the build system or code.

We'd like you to provide us with two products:

1) A writeup of:

- a) how you built the code
- b) what problems you identified along the way in building or running the code and what you did (or would do) to try to solve them
- c) reasoning behind any performance or efficiency changes you see (or made).
- d) a clear summary of how much performance or efficiency impact your code changed had (or could have).

2) A set of changes, in the form of a diff, to the code or build system you made while working. Create this by running `git diff`.

Write as if your audience is the original developer of the code and you want to help them learn - when they return from vacation - what you learned in improving their code.

Again, we are not looking for perfection. None of us are experts in C++, we're all learning. Do your best, bring your full self, and have fun with this! We're excited to see what you come up with!


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

You can clean the downloaded mason dependencies with:

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


The `./data/places.txt` sample file, for reference, was created like:


```bash
wget http://download.geofabrik.de/north-america/us/washington-latest.osm.pbf
mason install osmium-tool 1.7.1
mason link osmium-tool 1.7.1
# create config for osmium-tool to only export data with the `name` tag
echo '{ "include_tags": [ "name" ] }' > export-config.json
# convert OSM data to ldjson with http://docs.osmcode.org/osmium/latest/osmium-export.html
./mason_packages/.link/bin/osmium export washington-latest.osm.pbf -o washington-latest.json -f geojsonseq -c export-config.json
# convert to simplified csv for the downstream example project to use (avoids needing to parse geojson)
node scripts/geojson2csv.js washington-latest.json > ./data/places.txt
```

The assumption should be that `nearest-places` will be deployed to production and used to process much larger, likely GB size, csv files.
