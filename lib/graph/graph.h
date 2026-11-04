#pragma once
#include <bits/stdc++.h>
#include "../csv/csv_parser.h"
using namespace std;

class Graph
{
    int v = 0;
    int e = 0;
    map<Game,vector<pair<Game, int>>> adj; //adjacency list
public:
    Graph();
    std::map<Game, vector<pair<Game, int>>>::iterator findNodeFromID(int gameid);
    int V();
    int E();

    void print();

};