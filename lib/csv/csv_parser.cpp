#include "csv_parser.h"
#include <fstream>    
#include <sstream>    
#include <iostream>
#include <stack>

// Orders games by id, so Game can be stored in std::set / std::map.
bool Game::operator<(const Game& other) const
{
    return title < other.title;
}

Game::Game(std::string gametitle)
{
    title = gametitle;
}

void Game::printmask()
{
    unsigned __int128 tmp = mask;
    std::stack<int> stk;
    while (tmp > 0)
    {
        stk.push(tmp % 2);
        tmp /= 2;
    }
    while(!stk.empty()){
        std::cout << stk.top();
        stk.pop();
    }
}

// ---------- CSV parsing (from anthony_branch) ----------

// Reads the next CSV field from ss into `field`, handling quoted fields
// that contain commas. Quotes are removed from the result.
// Returns true if a field was read, false if the line had nothing left.
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
        std::getline(ss, field, ',');     
    }
    return true;
}


bool loadGames(const std::string& path, std::unordered_map<int, Game>& games, int n)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Could not open " << path << '\n';
        return false;
    }

    std::string line;
    std::getline(file, line);          // skip the header row

    for (int i = 0; i < n; i++)
    {
        std::getline(file, line);
        // Strip the '\r' left by Windows line endings.
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        Game g;
        std::string field;

        // Column 1: AppID (text -> int)
        readField(ss, field);
        g.id = std::stoi(field);

        // Columns 2-4: copied as-is
        readField(ss, g.title);
        readField(ss, g.releaseDate);
        readField(ss, g.owners);

        // Column 5: Publishers, split on commas into the vector
        readField(ss, field);
        std::stringstream listStream(field);
        std::string piece;
        while (std::getline(listStream, piece, ','))
        {
            g.publishers.push_back(piece);
        }
        g.v_index = i;
        games.emplace(g.id, g);
    }
    return true;
}
