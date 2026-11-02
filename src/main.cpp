
#include <iostream>
#include <fstream>
#include <vector>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

int main() {
    std::cout << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";


    std::ifstream graphf("lib/graph/graph.txt");
    if (!graphf)
    {
        // No saved graph yet: build it from the CSV.
        Graph steamGraph; //default constructor includes parser printing
        steamGraph.print();

        // TODO: add games to steamGraph (depends on Graph's interface)
    }
    else
    {
        // TODO: read the saved graph from graph.txt
    }
    return 0;
}
