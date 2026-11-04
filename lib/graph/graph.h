#pragma once
#include <bits/stdc++.h>
#include "../csv/csv_parser.h"
using namespace std;

class Graph
{
    int v = 0; // |V|
    int e = 0; // |E|
    int k = 8; // max connections per game
    int t = 10; // max tags per game

    map<Game,vector<pair<Game, int>>> adj; //adjacency list
    std::unordered_map<int, Game> games; //loaded games from csv
public:
    Graph(std::unordered_map<int, Game>& gamelist);
    Graph(std::unordered_map<int, Game>& gamelist, std::ifstream& graphf);
    std::map<Game, vector<pair<Game, int>>>::iterator findNodeFromID(int gameid);
    std::map<Game, vector<pair<Game, int>>>::iterator findNodeFromTitle(string& title);

    int V();
    int E();

    void print();

};