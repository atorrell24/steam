cmake_minimum_required(VERSION 4.1.2)
project(steam)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/graph)

add_executable(steam
        lib/graph/graph.cpp
        lib/graph/graph.h
        src/main.cpp
        lib/csv/csv_parser.h
        lib/csv/csv_parser.cpp)
