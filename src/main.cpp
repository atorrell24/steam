
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

int main() {
    std::cout << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";

    std::ifstream graphf("lib/graph/graph.csv");
    if (graphf) //change to !graphf for final
    {
        // No saved graph yet: build it from the CSV.
        std::unordered_map<int, Game> games;
        if (!loadGames("lib/csv/sample.csv", games)) {
            return 1;   
        }
        std::cout << "Loaded " << games.size() << " games\n";

        // TEMPORARY: confirms the parser still works after the merge. Remove once the graph is connected.
        for (const auto& entry : games) {
            const Game& g = entry.second;
            std::cout << g.id << " | " << g.title
                      << " | publishers: " << g.publishers.size() << '\n';
        }

        // TODO: add games to steamGraph (depends on Graph's interface)
    }
    else
    {
        // TODO: read the saved graph from graph.csv
    }
    return 0;
}
