#include "protocol.h"

#include <iostream>
#include <sstream>     
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <chrono> 
#include <stdexcept>

#include "../third_party/nlohmann/json.hpp"

using json = nlohmann::json;

namespace {

void reply(const json& obj)
{
    std::cout  << obj.dump(-1, ' ', false, json::error_handler_t::replace) << std::endl;
}


const std::vector<std::string> ALGOS = {"bfs", "dijkstra"};



std::string strip(const std::string& str) {
    //finds the first character that isn’t whitespace, scanning from the front.
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();
    
    return (start < end) ? std::string(start, end) : "";
}

std::string toLower(const std::string& str)
{
    std::string copy_str = str;
    for (char& ch : copy_str)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            ch +='a' - 'A';
        }
    }
    return copy_str;
}
json gameToJson(const Game& g)
{
    return json{
        {"id", g.id}, {"title", g.title}, {"releaseDate", g.releaseDate},
        {"owners", g.owners}, {"publishers", g.publishers}, {"tags", g.tags},
    };
}
}




void runCommandLoop(Graph& graph)
{
    reply(json{
        {"status", "ready"},
        {"games",  graph.V()},    // replace with your partner's actual function names
        {"edges",  graph.E()},
    });
    
    
    std::string line;
    
    while (std::getline(std::cin, line))
    {
        line = strip(line);
        

        if (line.empty()) 
        {
            continue; 
        }

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        std::string rest; 
        std::getline(iss >> std::ws, rest);

        if (command == "quit")
        {
            break;
        }

        else if(command == "algos")
        {
            reply(json{{"algos", ALGOS}});
        }
        
        else if (command == "search")
        {
            if (rest.empty())
            {
                reply(json{{"error", "search needs text."}});
                continue;
            }
            std::string query = toLower(rest);
            json results = json::array();

            for (const auto& entry : graph.getGames())
            {
                const Game& game = entry.second;
                if (toLower(game.title).find(query) != std::string::npos) 
                {
                    results.push_back({{"id", game.id}, {"title", game.title}});
                    if (results.size() == 20)
                    {
                        break; 
                    }
                }
            }
            reply(json{{"results", results}});
        }

        else if (command == "path")
        {
            std::istringstream argStream(rest);
            std::string algo, fromText, toText, place_holder;
            

            if (!(argStream >> algo >> fromText >> toText))
            {
                reply(json{{"error", "path needs exactly 3 inputs."}});
                continue;
            }
            
            else if (argStream >> place_holder)
            {
                reply(json{{"error", "path has too many arguments, must be exactly 3."}});
                continue;
            }

            if (std::find(ALGOS.begin(), ALGOS.end(), algo) == ALGOS.end())
            {
                reply(json{{"error", "algos must match bfs and dijkstra exactly."}});
                continue;
            }      
            int fromId = 0;
            int toId = 0;
            try
            {
                size_t usedFrom = 0, usedTo = 0;
                fromId = std::stoi(fromText, &usedFrom);
                toId = std::stoi(toText, &usedTo);
                if (usedFrom != fromText.size() || usedTo != toText.size())
                {
                    throw std::invalid_argument("trailing characters");   
                }
            }
            catch (const std::exception&)
            {
                reply({{"error", "ID's must be numbers."}});
                continue; 
            }
            const auto& games = graph.getGames();
            if (games.find(fromId) == games.end() || games.find(toId) == games.end())
            {
                reply(json{{"error", "unknown game id"}});
                continue;
            }
            std::vector<const Game*> result;
            size_t visitedCount = 0;
            auto start = std::chrono::steady_clock::now();
            if (algo == "bfs")
                result = graph.bfs(fromId, toId, visitedCount);
            else
                result = graph.dijkstra(fromId, toId, visitedCount);
            auto stop = std::chrono::steady_clock::now();
            long long timeUs =
                std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            // Path nodes, start to end (empty when there's no path).
            json pathNodes = json::array();
            for (const Game* g : result)
                pathNodes.push_back(gameToJson(*g));

            // One edge per consecutive pair; the weight comes from the first node's neighbor list.
            json edges = json::array();
            for (size_t i = 0; i + 1 < result.size(); i++)
            {
                int a = result[i]->id;
                int b = result[i + 1]->id;
                int weight = -1;                     // -1 only if the edge isn't found (would be a bug)
                if (const auto* neighbors = graph.getNeighbors(a))
                {
                    for (const auto& n : *neighbors)
                    {
                        if (n.first->id == b) { weight = n.second; break; }
                    }
                }
                edges.push_back({{"from", a}, {"to", b}, {"weight", weight}});
            }

            reply(json{
                {"algo", algo},
                {"found", !result.empty()},
                {"time_us", timeUs},
                {"visited", visitedCount},
                {"path", pathNodes},
                {"edges", edges},
            });


           
        }


        else
        {
            reply(json{{"error", "unknown command: " + command}});
        }
        

         

    }
}