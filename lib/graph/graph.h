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

    map<Game,vector<pair<const Game*, int>>> adj; //adjacency list
    unordered_map<int, Game> games; //loaded games from csv
    vector<const Game*> index_table;
public:

    Graph(unordered_map<int, Game>& gamelist);
    Graph(unordered_map<int, Game>& gamelist, ifstream& graphf);
    map<Game, vector<pair<const Game*, int>>>::iterator findNodeFromID(int gameid);
    map<Game, vector<pair<const Game*, int>>>::iterator findNodeFromTitle(string& title);
    vector<const Game*> dijkstra(int from, int to);
    vector<const Game*> bfs(int from, int to);

    int V();
    int E();
    void print();

};