import streamlit as st
#import json

from streamlit_agraph import agraph, Node, Edge, Config
# from time import time

@st.cache_data
def make_graph(node_dict, edge_dict, game1, game2):
    nodes = []
    edges = []
    id_title_dict = {}

    for data in node_dict:
        name = data["title"]
        id = data["id"]
        node_color = "#808080"

        if data["id"] == game2 : node_color = "#FF0000" #red
        if data["id"] == game1 : node_color = "#023E8A" #blue

        id_title_dict[id] = name
        nodes.append(Node(id=name, label=name, symbolType='diamond',color=node_color))
    for data in edge_dict:
        name = id_title_dict[data["from"]]
        node_target = id_title_dict[data["to"]]
        edges.append(Edge(source=name, target=node_target))

    return [nodes, edges]

graph_config = Config(width="stretch",
                height=650,
                directed=False,
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


