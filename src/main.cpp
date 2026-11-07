
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

#include "../lib/protocol/protocol.h" //Frontend , command loop

int main() {
    int n = 90; // number of games
    std::cerr << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";
    std::unordered_map<int, Game> games;
    if (!loadGames("lib/csv/steamgames.csv", games, n)) //populates Game vector
    {
        cerr << "could not load games";
        return 1;
    }
    std::cerr << "Loaded " << games.size() << " games\n";
    
    std::ifstream graphf("lib/graph/graph.csv");
    if (!graphf) //change to !graphf for final
    {
        // No saved graph yet: build it from the CSV.
        Graph steamGraph(games);
        runCommandLoop(steamGraph);
       
    }
    else
    {
        Graph steamGraph(games, graphf);
        runCommandLoop(steamGraph);
        
    }
    return 0;
}
