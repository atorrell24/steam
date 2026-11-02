#include "graph.h"
#include "../csv/csv_parser.h"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
random_device rd;   // non-deterministic generator
mt19937 gen(rd());
uniform_int_distribution<> dist(0, 127); // distribute results between 0 and 127 inclusive.


int popcount128(__int128 x) {
    //popcount works on 64bit ll at most, create custom 128bit popcount
    //return number of 1s in bit string
    ll low = x;
    ll high = x >> 64;
    return __builtin_popcountll(low) + __builtin_popcountll(high);
}

vector<string> generateTagList() {
    vector<string> tags;
    ifstream tagf("lib/graph/tags.txt");
    if (!tagf)
    {
        cout << "error opening file";
        throw runtime_error("uh oh");
    }
    string tag;
    while (getline(tagf, tag))
    {
        tags.emplace_back(tag);
    }
    sort(tags.begin(), tags.end());
    return tags;
}

Graph::Graph()
{
    int n = 5; // number of games
    int k = 2; // max connections per game

    vector<Game> games(n); //initialize with parser
    vector<string> ls = generateTagList(); //tag, index = vector index


    //set tags+masks
    for (auto& i : games)
    {
        for (int j = 0; j < 10; j++)
        {
            int rngtag = dist(gen);
            i.tags.emplace_back(ls[rngtag]);
            i.mask |= (__int128)1 << rngtag;
        }
    }
    //construct adjacencies/weights
    for (int i = 0; i < n; ++i) {
        // {number of different tags, id of the other game}
        vector<pair<Game, int>> weights;

        for (int j = 0; j < n; ++j) {
            if (j == i) continue;
            //xor only sets a bit if exactly 1 game has a tag
            int difference = popcount128(games[i].mask ^ games[j].mask);
            weights.emplace_back(games[j], difference);
        }
        //only interested in first k connections
        partial_sort(weights.begin(), weights.begin() + k, weights.end());
        vector<pair<Game, int>> tmp(weights.begin(), weights.begin() + k);
        adj[std::move(games[i])].assign(weights.begin(), weights.begin() + k);
    }
    v = adj.size();
    e = adj.size() * k;

    //write to .txt here
}

std::map<Game, vector<pair<Game, int>>>::iterator Graph::findNode(int gameid)
{
    Game tmp(gameid);
    return adj.find(tmp);
}

int Graph::V()
{
    return v;
}

int Graph::E()
{
    return e;
}

void Graph::print()
{
    for (auto& i : adj)
    {
        cout << i.first.title << " - ";
        for (auto& j : i.second)
        {
            cout << j.first.id << " " << j.second << ", ";
        }
        cout << endl;
    }
}


