
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

int main() {
    int n = 125855; // number of games. 125855
    std::cout << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";
    std::unordered_map<int, Game> games;
    if (!loadGames("lib/csv/steamgames.csv", games, n)) //populates Game vector
    {
        cout << "could not load games";
        return 0;
    }
    std::cout << "Loaded " << games.size() << " games\n";
    std::ifstream graphf("lib/graph/graph.csv");

    Graph steamGraph ((!graphf) ? Graph(games) : Graph(games, graphf)); //if no graph.csv, build from db, else, build from graph.csv
    steamGraph.print();
    // auto cost = steamGraph.dijkstra("ARCHiTECTUS", "Aeroplanoui");
    // for (auto& i : cost)
    // {
    //     cout << i << endl;
    // }
    return 0;
    //ENOH, POLAR, ARCHiTECTUS, Aeroplanoui
}
