import streamlit as st
import json

from streamlit_agraph import agraph, Node, Edge, Config, return_value
import requests
import numpy as np
import pandas as pd
from time import time

st.title('Steam Game Recommender')

#todo
#make basic website layout
    #graph in center
    #sidebar for selecting games & algorithm
    #return loaded graph
    #

col1, col2 = st.columns([3, 1])
col1.subheader("Graph Display")
#col1.line_chart(df)

col2.subheader("Game & Algorithm Info")
#col2.write(df)


@st.cache_data
def load_data_a(returned_graph):
    df = pd.read_json(returned_graph)
    return df

df = load_data_a(".json file TBD") #NEEDS FILE TO READ

#converts .json into a dictionary
    #we could do a csv -> json as well
#preferable order = AppID, Name, Edge(next in order or if the adjacent nodes all point to game #1/#2,
                    #Release Date,Estimated owners,Publishers,Genres
dict_convert = json.loads(df)


def make_graph(dict_convert):
    nodes = []
    edges = []

    for data in dict_convert:
        name = data[1]
        node_target = data[2] #None if no out-going edge
        node_color = "#808080"

#!!!!change game# names
        if name == "game1" : node_color = "#FF0000" #red
        if name == "game2" : node_color = "#023E8A" #blue

        #create node
        #create edge if target != None
        nodes.append(Node(id=name, symbolType='diamond',color=node_color))
        if node_target != "None":
            edges.append(Edge(source=name, target=node_target))

        return [nodes, edges]

graph_config = Config(width=750,
                height=950,
                directed=True,
                physics=True,
                hierarchical=False,
                highlightColor='#FF00FF',
                nodeHighlightBehavior=True)
                #node={'labelProperty':'label','renderLabel':False})

graph_nodes, graph_edges = make_graph(dict_convert)
return_value = agraph(nodes = graph_nodes,
                      edges = graph_edges,
                      config = graph_config)


