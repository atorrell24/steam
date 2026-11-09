S.T.E.A.M. : System for Thematically Evaluating Arrays of Media

Finds a path between two Steam games through a similarity graph, using BFS or Dijkstra, and compares how long each algorithm takes. The graph and algorithms are in C++; the UI is Python (Streamlit).

Project layout
src/main.cpp              entry point: load games, build/load graph, start command loop
lib/csv/                  CSV parser (Game struct, loadGames)
lib/graph/                graph, tags, BFS, Dijkstra
lib/protocol/             command loop: reads commands on stdin, replies with JSON on stdout
lib/third_party/nlohmann/ json.hpp (v3.11.3)
frontend/UI.py            Streamlit UI
frontend/backend_client.py  Python client that starts and talks to the C++ backend
frontend/fake_backend.py  stand-in backend with hard-coded data (for UI work without C++)
Requirements
g++ with C++17 (MinGW on Windows; the code uses GCC-only features)
Python 3 with: pip install streamlit streamlit-agraph pandas
Build

From the project root:

g++ -std=c++17 -O2 -Wall src\main.cpp lib\csv\csv_parser.cpp lib\graph\graph.cpp lib\protocol\protocol.cpp -o steam_backend.exe

Rebuild after pulling any C++ change , the UI always launches steam_backend.exe in the project root.

First run (builds the graph cache)

The first run builds lib/graph/graph.csv, which takes a long time for the full dataset. Do it once from a terminal, not from the UI:

steam_backend.exe          (PowerShell: .\steam_backend.exe)

Wait for the {"status":"ready",...} line, then type quit. Later runs load the cache in seconds.

Delete lib/graph/graph.csv whenever n in main.cpp changes or the graph/tag code changes, so it gets rebuilt.

Run the UI

From the project root:

streamlit run frontend/UI.py
Rules for the C++ backend
stdout is for JSON replies only. Any debug output must use std::cerr, or the UI fails with "backend sent non-JSON output".
Check with steam_backend.exe 2>nul (PowerShell: .\steam_backend.exe 2>$null): the ready line must be the only output.
Commands (backend protocol)
Command	Reply
search <text>	{"results": [{"id", "title"}, ...]} (max 20)
algos	{"algos": ["bfs", "dijkstra"]}
path <algo> <fromId> <toId>	{"algo", "found", "time_us", "visited", "path": [...], "edges": [...]}
quit	exits

Errors reply {"error": "<message>"}. Full details in PROTOCOL.md.
