cmake_minimum_required(VERSION 4.1.2)
project(steam)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/graph
                    lib/csv)

add_executable(steam
        src/main.cpp
        lib/graph/graph.cpp
        lib/graph/graph.h
        lib/csv/csv_parser.h
        lib/csv/csv_parser.cpp)
