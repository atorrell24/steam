#pragma once
#include <bits/stdc++.h>
#include <../csv/csv_parser.h>
using namespace std;

class Graph
{
    map<Game,vector<pair<Game, int>>> adj; //adjacency list
public:
    Graph();

};