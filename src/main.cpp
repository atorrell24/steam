
#include <iostream>
#include <fstream>
#include <vector>

#include "../lib/csv/csv_parser.h"   // Game, loadGames
#include "../lib/graph/graph.h"      // Graph

int main() {
    std::cout << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA\n";

    std::ifstream graphf("lib/graph/graph.csv");
    if (graphf) //change to !graphf for final
    {
        // No saved graph yet: build it from the CSV.
        Graph steamGraph; //default constructor includes parser printing
        // steamGraph.print();
    }
    else
    {
        // TODO: read the saved graph from graph.csv
    }
    return 0;
}
