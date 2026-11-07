import streamlit as st
import json

from streamlit_agraph import agraph, Node, Edge, Config
import requests
import numpy as np
import pandas as pd
from time import time

# @st.cache_data
# def load_data_a(returned_graph):
#     df = pd.read_json(returned_graph)
#     return df
#
# df = load_data_a("test.output.txt") #NEEDS FILE TO READ
#
# #converts .json into a dictionary
#     #we could do a csv -> json as well
# #preferable order = AppID, Name, Edge(next in order or if the adjacent nodes all point to game #1/#2,
#                     #Release Date,Estimated owners,Publishers,Genres
# dict_convert = json.loads(df)

@st.cache_data
def make_graph(node_dict, edge_dict, game1, game2):
    nodes = []
    edges = []
    id_title_dict = {}

    for data in node_dict:
        name = data["title"]
        id = data["id"]
        node_color = "#808080"

    #!!!!change game# names + make it a fstring
        if data["id"] == game2 : node_color = "#FF0000" #red
        if data["id"] == game1 : node_color = "#023E8A" #blue

        #create node
        #create edge if target != None
        id_title_dict[id] = name
        nodes.append(Node(id=name, label=name, symbolType='diamond',color=node_color))
    for data in edge_dict:
        name = id_title_dict[data["from"]]
        node_target = id_title_dict[data["to"]]
        edges.append(Edge(source=name, target=node_target))

    return [nodes, edges]

graph_config = Config(width="stretch",
                height=750,
                directed=True,
                physics=True,
                hierarchical=False,
                highlightColor='#FF00FF',
                nodeHighlightBehavior=True)
                #node={'labelProperty':'label','renderLabel':False})

# graph_nodes, graph_edges = make_graph(dict_convert)
#
# return_value = agraph(nodes = graph_nodes,
#                       edges = graph_edges,
#                       config = graph_config)


