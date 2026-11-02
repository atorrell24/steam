
#include <iostream>
#include <fstream>
#include <vector>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

int main() {
    std::cout << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";

    Graph steamGraph;   

    std::ifstream graphf("lib/graph/graph.txt");
    if (!graphf)
    {
        // No saved graph yet: build it from the CSV.
        std::vector<Game> games;
        if (!loadGames("lib/csv/sample.csv", games)) {
            return 1;   
        }
        std::cout << "Loaded " << games.size() << " games\n";

        // TEMPORARY: confirms the parser still works after the merge. Remove once the graph is connected.
        for (const Game& g : games) {
            std::cout << g.id << " | " << g.title
                      << " | publishers: " << g.publishers.size() << '\n';
        }

        // TODO: add games to steamGraph (depends on Graph's interface)
    }
    else
    {
        // TODO: read the saved graph from graph.txt
    }
    return 0;
}
