
#pragma once

#include <string>
#include <vector>

struct Game
{
    int id = 0;
    __int128 mask =0;
    std::string title;
    std::string releaseDate;
    std::string owners;
    std::vector<std::string> publishers;
    std::vector<std::string> tags;
    Game() = default;
    Game(int gameid);  
    bool operator<(const Game& other) const;

};

bool loadGames(const std::string& path, std::vector<Game>& games);
