#ifndef STEAM_CSV_PARSER_H
#define STEAM_CSV_PARSER_H
#endif //STEAM_CSV_PARSER_H
#pragma once
#include <bits/stdc++.h>

using namespace std;

struct Game {
    int id = 0;
    __int128 mask = 0;
    vector<string> tags;
    string releaseDate;
    string title;
    string owners;
    std::vector<std::string> publishers;
    Game() = default;
    Game(int gameid);
    bool operator<(const Game& other) const;
};