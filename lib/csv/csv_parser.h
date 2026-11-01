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
    bool operator<(const Game& other) const;
};