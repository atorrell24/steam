#include "csv_parser.h"

bool Game::operator<(const Game& other) const
{
    return id < other.id;
}
