import streamlit as st
#import json
import sys
from pathlib import Path

from streamlit_agraph import agraph, Node, Edge, Config
import pandas as pd
from time import time

from frontend_graph_make import make_graph, graph_config
from backend_client import Backend, BackendError, BackendDiedError

#------To Run in local terminal: streamlit run frontend/UI.py
@st.cache_resource
def load_data():
    here = Path(__file__).resolve().parent   # the folder this file is in
    steam_dir = here.parent
    b = Backend([str(steam_dir / "steam_backend.exe")],
                log_path=str(here / "backend_log.txt"),
                cwd=str(steam_dir))
    return b

b = load_data()
print()
game1 = {}
game2 = {}
algo = ""
df = 0

#Ensures Consistency Across every re-run action
if "algo_result" not in st.session_state:
    st.session_state.algo_result = None
if "algo_time" not in st.session_state:
    st.session_state.algo_time = None
if "algo_visited" not in st.session_state:
    st.session_state.algo_visited = None
if "graph_data" not in st.session_state:
    st.session_state.graph_data = None
if "active_tab" not in st.session_state:
    st.session_state.active_tab = "Games & Algorithms"

#HomePAge Division of left=graph and right=info
st.set_page_config(layout="wide")
col1, col2 = st.columns([3, 1])

#Information column
with col2:
    st.space("medium")
    tab1, tab2 = st.tabs(["Games & Algorithms", "Additional Info"], key="active_tab")
    with tab1: #Games & Algorithms
    #Game Title Selections
    #!!! needs error statements for if search doesn't find game--------------------------*****
        game_title1 = st.text_input("Game Title #1", key="game1_input")
        if game_title1 != "":
            searched = b.search(game_title1)
            if not searched:
                st.warning('Game Not Found', icon="⚠️")
            else:
                game1 = searched[0]
                st.markdown(f"Game Selected: {game1["title"]}")
        game_title2 = st.text_input("Game Title #2", key="game2_input")
        if game_title2 != "":
            searched = b.search(game_title2)
            if not searched:
                st.warning('Game Not Found', icon="⚠️")
            else:
                game2 = searched[0]
                st.markdown(f"Game Selected: {game2["title"]}")
    #Algorithm Selection:
        algo_options = ["Dijkstra", "BFS"]
        selection = st.segmented_control("Select Algorithm", algo_options, selection_mode="single")
        algo = selection
        st.space("small")
    #Button to Run Algorthm
        if st.button("Run Algorithm", type="primary", width="stretch"):
            if (game_title1 == "") or (game_title2 == "") or (selection is None):
                st.warning('Empty Input Detected', icon="⚠️")
            else:
                #verify game1 & 2 exist by calling search
                #call path(algo, from_id, to_id):
                if (algo == "Dijkstra"):
                    st.session_state.algo_result = b.path("dijkstra", int(game1["id"]), int(game2["id"]))
                elif (algo == "BFS"):
                    st.session_state.algo_result = b.path("bfs", int(game1["id"]), int(game2["id"]))

                df = st.session_state.algo_result["path"]
                df2 = st.session_state.algo_result["edges"]

                graph_nodes, graph_edges = make_graph(df, df2, int(game1["id"]), int(game2["id"]))
                st.session_state.graph_data = make_graph(df, df2, int(game1["id"]), int(game2["id"]))
                st.session_state.algo_time = st.session_state.algo_result["time_us"]
                st.session_state.algo_visited = st.session_state.algo_result["visited"]
        #Time Results Displayed
        if st.session_state.algo_time is not None:
            st.markdown(f"### Performance Metrics")
            st.markdown(f"**Time Taken:** {st.session_state.algo_time} μs")
            st.markdown(f"**Nodes Touched:** {st.session_state.algo_visited}")

    with tab2: #Additional Info
        st.markdown("""
            <style>
            div[data-testid="stExpander"] details summary p {
                font-size: 18px;
                font-weight: bold;
            }
            </style>
            """, unsafe_allow_html=True)
        #Listed all info in expanded sections
        if st.session_state.algo_result is not None:
            for game in st.session_state.algo_result["path"]:
                with st.expander(game["title"]):
                    st.write(f"Game ID: {game["id"]}")
                    pubs = "Publisher(s): "
                    for publisher in game["publishers"]:
                        pubs += f"{publisher}, "
                    pubs = pubs[:-2]
                    st.write(pubs)
                    st.write(f"Release Date: {game["releaseDate"]}")
                    st.write(f"Bought By: {game["owners"]} players")
                    tag_list = "Tags:  "
                    for tag in game["tags"]:
                        tag_list += f"{tag}, "
                    tag_list = tag_list[:-2]
                    st.write(tag_list)

with col1: #GRAPH DISPLAY :)
    st.title('S.T.E.A.M', text_alignment="center")
    st.subheader("System for Thematically Evaluating Arrays of Media", text_alignment="center")
    st.markdown("Bridging Games You Know to Find Games You Don't!", text_alignment="center")
    if st.session_state.graph_data is not None:
        nodes, edges = st.session_state.graph_data
        final_graph = agraph(nodes=nodes, edges=edges, config=graph_config)


