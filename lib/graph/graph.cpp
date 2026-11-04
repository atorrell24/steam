#include "graph.h"
#include "../csv/csv_parser.h"
#include <bits/stdc++.h>

#include <utility>
using namespace std;
using ll = long long;
random_device rd;   // non-deterministic generator
mt19937 gen(rd());
uniform_int_distribution<> dist(0, 127); // distribute results between 0 and 127 inclusive.

int popcount128(unsigned __int128 x) {
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

Graph::Graph(std::unordered_map<int, Game>& gamelist) : games(std::move(gamelist))
{
    vector<string> ls = generateTagList(); //index of tag = vector index

    //set tags+masks
    for (auto& i : games)
    {
        std::set<int> addedtags;
        while (i.second.tags.size() < t)
        {
            int rngtag = dist(gen);
            if (addedtags.count(rngtag) == 0)
            {
                i.second.tags.emplace_back(ls[rngtag]);
                i.second.mask |= (unsigned __int128)1 << rngtag;
                addedtags.insert(rngtag);
            }
        }
    }

    // for (auto& i : games)
    // {
    //     cout << i.title << " - ";
    //     i.printmask();
    //     cout << endl;
    // }

    //construct adjacencies/weights
    for (auto& i : games)
    {
        // {game, weight}
        vector<pair<Game, int>> weights;

        for (auto& j : games)
        {
            if (j.first == i.first) continue;
            //xor only sets a bit if exactly 1 game has a tag
            int difference = popcount128(i.second.mask ^ j.second.mask);
            weights.emplace_back(j.second, difference);
        }
        //only interested in first k connections
        partial_sort(weights.begin(), weights.begin() + k, weights.end(), [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });
        std::move(weights.begin(), weights.begin() + k, std::back_inserter(adj[i.second]));
    }
    v = adj.size();
    e = adj.size() * k;

    //write to .csv
    std::ofstream graphf("lib/graph/graph.csv");
    if (!graphf.is_open())
    {
        std::cerr << "error writing to graph.csv";
        return;
    }
    for (auto& i : adj)
    {
        graphf << i.first.id << ",";
        const char* sep = "";
        for (auto& j : i.second)
        {
            graphf << sep << j.first.id << "," << j.second;
            sep = ",";
        }
        graphf << endl;
    }
}

Graph::Graph(std::unordered_map<int, Game>& gamelist, std::ifstream& graphf) : games(std::move(gamelist))
{
    string line;
    while (getline(graphf, line))
    {
        stringstream ss(line);
        string node;
        getline(ss, node, ','); //source node
        Game source = games[std::stoi(node)];
        for (int i = 0; i < k; i++)
        {
            getline(ss, node, ',');
            string weight;
            getline(ss, weight, ',');
            pair<Game, int> connection = {games[std::stoi(node)], std::stoi(weight)};
            adj[source].emplace_back(connection);
        }
    }
}

std::map<Game, vector<pair<Game, int>>>::iterator Graph::findNodeFromID(int gameid)
{
    return adj.find(games[gameid]);
}

std::map<Game, vector<pair<Game, int>>>::iterator Graph::findNodeFromTitle(string& title)
{
    Game tmp(title);
    return adj.find(tmp);
};


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
        cout << i.first.title << " (" << i.first.id << ") - ";
        for (auto& j : i.second)
        {
            cout << j.first.title << " " << j.second << ", ";
        }
        cout << endl;
    }
}


