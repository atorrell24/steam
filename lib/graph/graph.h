#pragma once
#include <bits/stdc++.h>
#include "../csv/csv_parser.h"
using namespace std;

class Graph
{
    int v = 0; // |V|
    int e = 0; // |E|
    int k = 8; // max connections per game
    size_t t = 10; // max tags per game

    map<Game,vector<pair<const Game*, int>>> adj; //adjacency list
    unordered_map<int, Game> games; //loaded games from csv
    vector<const Game*> index_table;
public:

    Graph(unordered_map<int, Game>& gamelist);
    Graph(unordered_map<int, Game>& gamelist, ifstream& graphf);
    
    const unordered_map<int, Game>& getGames() const;
    const vector<pair<const Game*, int>>* getNeighbors(int gameid) const;
    
    
    map<Game, vector<pair<const Game*, int>>>::iterator findNodeFromID(int gameid);
    
    
    vector<const Game*> dijkstra(int from, int to, size_t& visitedCount);
    vector<const Game*> bfs(int from, int to, size_t& visitedCount);

    int V();
    int E();
    void print();

};