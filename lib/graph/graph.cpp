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

Graph::Graph(){}

Graph::Graph(unordered_map<int, Game>& gamelist) : games(move(gamelist))
{
    vector<string> ls = generateTagList(); //index of tag = vector index
    v = games.size();
    e = games.size() * k;
    index_table.resize(v);
    //set tags+masks
    for (auto& i : games)
    {
        set<int> addedtags;
        while (i.second.tags.size() < t) //ensures 10 unique tags
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

    // {game ptr, weight}
    vector<pair<const Game*, int>> weights;
    weights.reserve(v);
    //construct adjacencies/weights
    for (auto& i : games)
    {
        for (auto& j : games)
        {
            if (j.first == i.first) continue;
            //xor only sets a bit if exactly 1 game has a tag
            int difference = popcount128(i.second.mask ^ j.second.mask);
            weights.emplace_back(&j.second, difference);
        }
        //only interested in first k connections, sort by weight
        partial_sort(weights.begin(), weights.begin() + k, weights.end(), [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });
        move(weights.begin(), weights.begin() + k, back_inserter(adj[i.second]));
        index_table[i.second.v_index] = &adj.find(i.second)->first; //index_table associates games with indices
        weights.clear();
    }

    //write to .csv
    ofstream graphf("lib/graph/graph.csv");
    if (!graphf.is_open())
    {
        cerr << "error writing to graph.csv";
        return;
    }
    for (auto& i : adj)
    {
        graphf << i.first.id << ",";
        const char* sep = "";
        for (auto& j : i.second)
        {
            graphf << sep << j.first->id << "," << j.second;
            sep = ",";
        }
        graphf << endl;
    }
}

Graph::Graph(unordered_map<int, Game>& gamelist, ifstream& graphf) : games(move(gamelist))
{
    v = games.size();
    e = games.size() * k;
    index_table.resize(v);
    string line;
    while (getline(graphf, line))
    {
        stringstream ss(line);
        string node;
        getline(ss, node, ','); //source node
        Game source = games[stoi(node)];
        for (int i = 0; i < k; i++)
        {
            getline(ss, node, ',');
            string weight;
            getline(ss, weight, ',');
            pair<Game*, int> connection = {&games[stoi(node)], stoi(weight)};
            adj[source].emplace_back(connection);
        }
        index_table[source.v_index] = &adj.find(source)->first; //index_table associates games with indices
    }
}

map<Game, vector<pair<const Game*, int>>>::iterator Graph::findNodeFromID(int gameid)
{
    return adj.find(games[gameid]);
}

map<Game, vector<pair<const Game*, int>>>::iterator Graph::findNodeFromTitle(string& title)
{
    Game tmp(title);
    return adj.find(tmp);
};

vector<const Game*> Graph::dijkstra(string from, string to)
{
    auto fromit = findNodeFromTitle(from); //iterator pointing to source node
    auto toit = findNodeFromTitle(to); //iterator pointing to target node

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; //each entry is <distance, gameID>
    set<int> visited; //stores visited gameIDs. distances here are locked.
    vector<int> costs(v, INT_MAX); //cheapest known cost from source to every node
    vector<int> preds(v, -1); //stores predecessor in shortest path
    vector<const Game*> res; //output

    costs[fromit->first.v_index] = 0;
    pq.emplace(0, fromit->first.id);

    for (int i = 0; i < v; i++) //handles potentially disconnected graph
    {
        while (!pq.empty())
        {
            auto currentit = findNodeFromID(pq.top().second); //iterator to current node.
            int du = pq.top().first; //minimum cost to reach u
            pq.pop();
            if (visited.count(currentit->first.id) != 0)
            {
                continue;
            }
            for (auto& i : currentit->second)
            {
                int idx = i.first->v_index; //index of game connected by edge
                int weight = i.second; //cost to reach game
                if (costs[idx] > du + weight)
                {
                    costs[idx] = du + weight;
                    pq.emplace(costs[idx], i.first->id);
                    preds[idx] = currentit->first.v_index;
                }
            }
            visited.insert(currentit->first.id); //distance is now locked
        }
    }
    if (costs[toit->first.v_index] == INT_MAX)
    {
        return {};
    }
    int l = toit->first.v_index;
    res.emplace_back(index_table[toit->first.v_index]);
    while (l != fromit->first.v_index)
    {
        res.emplace_back(index_table[preds[l]]);
        l = preds[l];
    }
    reverse(res.begin(), res.end());
    return res;
}

vector<const Game*> Graph::bfs(string from, string to)
{
    set<int> visited; //stores visited gamids
    vector<int> preds(v, -1); //stores indices of predecessors in bfs
    vector<const Game*> res; //bfs path
    auto fromit = findNodeFromTitle(from);
    auto toit = findNodeFromTitle(to);

    queue<int> q; //stores gameids
    visited.insert(fromit->first.id);
    q.push(fromit->first.id);

    while (!q.empty())
    {
        auto u = findNodeFromID(q.front()); //iterator to current node
        q.pop();
        for (auto& i : u->second)
        {
            if (visited.count(i.first->id) == 0)
            {
                visited.insert(i.first->id);
                q.push(i.first->id);
                preds[i.first->v_index] = u->first.v_index;
            }
            if (visited.count(toit->first.id) != 0)
            {
                break;
            }
        }
    }
    if (preds[toit->first.v_index] == -1)
    {
        return {};
    }
    int l = toit->first.v_index;
    res.emplace_back(index_table[toit->first.v_index]);
    while (l != fromit->first.v_index)
    {
        res.emplace_back(index_table[preds[l]]);
        l = preds[l];
    }
    reverse(res.begin(), res.end());
    return res;
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
        cout << i.first.title << " (" << i.first.id << ") - ";
        for (auto& j : i.second)
        {
            cout << j.first->title << " " << j.second << ", ";
        }
        cout << endl;
    }
}


