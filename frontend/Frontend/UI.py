import streamlit as st
import json
from streamlit_agraph import agraph, Node, Edge, Config
import requests
import numpy as np
import pandas as pd
from time import time


#------Run in local terminal with: streamlit run frontend/Frontend/UI.py

st.set_page_config(layout="wide")

col1, col2 = st.columns([3, 1])
with col1:
    st.title('Steam Game Recommender', text_alignment="center")
    #col1.line_chart(df)
with col2:
    st.space("xlarge")
    tab1, tab2 = st.tabs(["Games & Algorithms", "Additional Info"])
    with tab1:
    #Game Title Selections
        game_title1 = st.text_input("Game Title #1")
        game_title2 = st.text_input("Game Title #2")
    #Algorithm Selection:
        algo_options = ["Dijkstra", "BFS"]
        selection = st.segmented_control("Select Algorithm", algo_options, selection_mode="single")
        st.space("small")
    #Button to Run Algorthm
        if st.button("Run Algorithm", type="primary", width="stretch"):
            if (game_title1 == "") or (game_title2 == "") or (selection is None):
                st.warning('Empty Input Detected ', icon="⚠️")
            else:
                st.write("algorithm called")
                #call path(algo, from_id, to_id):
    #Time Results Displayed
        st.markdown(f"Time Taken: ")
        st.markdown(f"Nodes Touched: ")
    with tab2:
        with st.expander("Selected Game Info", expanded=True):
            st.write("grab node info")
        with st.expander("All Games"):
            st.write("all game names")

    #col2.write(df)