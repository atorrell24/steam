#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct Game
{
    int id = 0;
    unsigned __int128 mask = 0;
    std::string title;
    std::string releaseDate;
    std::string owners;
    std::vector<std::string> publishers;
    std::vector<std::string> tags;
    Game() = default;
    Game(int gameid);
    Game(std::string title);
    bool operator<(const Game& other) const;
    void printmask();
};

bool loadGames(const std::string& path, std::unordered_map<int, Game>& games);
