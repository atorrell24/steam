
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

#include "../lib/protocol/protocol.h" //Frontend , command loop

int main() {
    int n = 125855; // number of games. 125855
    std::cerr << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";
    std::unordered_map<int, Game> games;
    if (!loadGames("lib/csv/steamgames.csv", games, n)) //populates Game vector
    {
        cerr << "could not load games";
        return 1;
    }
    std::cerr << "Loaded " << games.size() << " games\n";
    
    std::ifstream graphf("lib/graph/graph.csv");

    Graph steamGraph ((!graphf) ? Graph(games) : Graph(games, graphf)); //if no graph.csv, build from db, else, build from graph.csv
    //testing stuff
    // auto result1 = steamGraph.bfs("Aeroplanoui", "INFRA");
    // auto result2 = steamGraph.dijkstra("Aeroplanoui", "INFRA");
    // for (auto& i : result1)
    //     cout << i->title << " " << endl;
    // cout << endl << endl;
    // for (auto& i : result2)
    //     cout << i->title << " " << endl;
    runCommandLoop(steamGraph);   

    return 0;

}
