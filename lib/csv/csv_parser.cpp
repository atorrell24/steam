#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

struct Game
{
    int appID = 0;
    std::string name;
    std::string releaseDate;
    std::string owners;
    std::vector<std::string> publishers;
    std::vector<std::string> tags;

};

bool readField(std::stringstream& ss, std::string& field)

{
    
    field.clear(); 

    int nextChar = ss.peek();
    
    if (nextChar == EOF)
    {
        return false;
    }

    if (nextChar == '"')
    {
        ss.get();
        std::getline(ss, field, '"');
        ss.ignore();
    }
    else
    {
        std::getline(ss,field, ',');
    }

    return true;
}



bool loadGames(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Could not open " << path << '\n';  
        return false;
    }

    std::string line;
    std::getline(file, line);  // skip header line in csv

    while (std::getline(file, line)) 
    {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        Game g;
        std::string field;

        //appID
        readField(ss, field);
        g.appID = std::stoi(field);

        //name of game
        readField(ss, g.name);
        //release date
        readField(ss, g.releaseDate);
        //owners
        readField(ss, g.owners);

        //publishers
        readField(ss, field);
        

    }
    return true;
}