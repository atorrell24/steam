
#pragma once

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

bool loadGames(const std::string& path, std::vector<Game>& games);