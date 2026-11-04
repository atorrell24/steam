import sys
import json



def reply(obj):
    json_string = json.dumps(obj)
    print(json_string, flush=True)

GAMES = {
    1654170: {
        "id": 1654170,
        "title": "Agony VR",
        "releaseDate": "Apr 5, 2023",
        "owners": "0 - 20000",
        "publishers": ["Ignibit", "Madmind Studio"],        # case: two publishers
        "tags": ["Horror", "Action", "VR"],
    },
    815170: {
        "id": 815170,
        "title": "Zombie World",
        "releaseDate": "Jun 1, 2018",
        "owners": "0 - 20000",
        "publishers": ["M.R.Studio"],
        "tags": ["Horror", "Action", "Survival"],
    },
    2113450: {
        "id": 2113450,
        "title": "Forests, Fields and Fortresses",          # case: commas in the title
        "releaseDate": "Apr 5, 2023",
        "owners": "0 - 20000",
        "publishers": ["9 Eyes Game Studio", "Stately Snail"],
        "tags": ["Strategy", "Casual", "Building"],
    },
    3292190: {
        "id": 3292190,
        "title": "버튜버 파라노이아 - Vtuber Paranoia",        # case: non-English characters
        "releaseDate": "Oct 31, 2024",
        "owners": "0 - 20000",
        "publishers": ["유진게임즈"],
        "tags": ["Horror", "Simulation"],
    },
    1946890: {
        "id": 1946890,
        "title": "Codename: Warlock Playtest",
        "releaseDate": "Mar 25, 2022",
        "owners": "0 - 0",
        "publishers": [],                                   # case: no publisher
        "tags": ["Action"],
    },
    1025440: {
        "id": 1025440,
        "title": "Fantasy General II",
        "releaseDate": "Sep 5, 2019",
        "owners": "100000 - 200000",
        "publishers": ["Slitherine Ltd."],
        "tags": ["Strategy", "Fantasy"],
    },
}

ready_message = {"status": "ready", "games": len(GAMES), "edges": 0 }
reply(ready_message)

ALGOS = ["bfs", "dijkstra"]





for line in sys.stdin:
    line = line.strip()
    
    if not line:
        continue
    parts = line.split(maxsplit=1)
    
    command = parts[0]

    if len(parts) > 1:
        rest = parts[1]
    else:
        rest = ""

    if command == "quit":
        break
    elif command == "algos":
        reply({"algos": ALGOS})
    elif command == "search":
        if not rest:                                      
            reply({"error": "search needs text."})
            continue 
        
        query = rest.lower()                     
        results = []                              

        for game in GAMES.values():               
            if query in game["title"].lower():   
                results.append({"id": game["id"], "title": game["title"]})
                if len(results) == 20:            
                    break                         
        reply({"results": results})

    elif command == "path":
        args = rest.split()
        if (len(args)) < 3  or len(args) > 3:
            reply({"error": "path needs exactly 3 inputs."})
            continue
        
        algo, from_text, to_text = args
        if  algo not in ALGOS:
            reply({"error": "algos must match bfs and dijkstra exactly."})
            continue
        try:
            from_id = int(from_text)
            to_id = int(to_text)
        except ValueError:
            reply({"error": "ID's must be numbers."})
            continue
       
        if from_id not in GAMES or to_id not in GAMES:      
            reply({"error": "unknown game id"})
            continue
        if from_id == to_id:
            path_ids = [from_id] 
        else:
            middle_id = None
            for game_id in GAMES:                         
                if game_id != from_id and game_id != to_id:
                    middle_id = game_id
                    break                                
            path_ids = [from_id, middle_id, to_id]
                            
        path_nodes = []
        for game_id in path_ids:
            path_nodes.append(GAMES[game_id])
                
        edges = []
        for i in range(len(path_ids) - 1):                
            edges.append({
                            "from": path_ids[i],
                            "to": path_ids[i + 1],
                            "weight": 2,                              
                        })
                            
        reply({
                "algo": algo,
                "found": True,
                "time_us": 412 if algo == "bfs" else 1290,    # different fake times, so the UI comparison shows a difference
                "visited": 4 if algo == "bfs" else 6,
                "path": path_nodes,
                "edges": edges,
            })


    else:
        reply({"error": f"unknown command: {command}"})
        



