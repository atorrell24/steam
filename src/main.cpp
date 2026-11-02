#include <iostream>
#include "csv_parser.h"
int main() {
    std::cout << "SYSTEM FOR THEMATICALLY EVALUATING ARRAYS OF MEDIA";
    
    std::vector<Game> games;

    if (!loadGames("lib/csv/sample.csv", games)) {
        return 1;  
    }

    
    std::cout << "Loaded " << games.size() << " games\n\n";

    
    for (const Game& g : games) {
        std::cout << g.appID << " | " << g.name
                  << " | publishers: " << g.publishers.size()
                  << " | tags: " << g.tags.size() << '\n';
    }
    return 0;

}