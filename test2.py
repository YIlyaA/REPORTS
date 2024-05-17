import networkx as nx
import random
import matplotlib.pyplot as plt
import math


def generate_acyclic_undirected_graph(n_nodes, edge_density):
    G = nx.Graph()

    # Add nodes
    G.add_nodes_from(range(n_nodes))

    # Add edges
    for i in range(n_nodes):
        for j in range(i + 1, n_nodes):
            if random.random() < edge_density:
                G.add_edge(i, j)
                # Check if adding this edge creates a cycle
                if nx.is_connected(G.to_undirected()):
                    # If a cycle is created, remove the edge
                    G.remove_edge(i, j)

    return list(G.edges)


def generate_acyclic_directed_graph(n_nodes, edge_density):
    G = nx.DiGraph()

    # Add nodes
    G.add_nodes_from(range(n_nodes))

    # Add edges
    for i in range(n_nodes):
        for j in range(i + 1, n_nodes):
            if random.random() < edge_density:
                G.add_edge(i, j)
                # Check if adding this edge creates a cycle
                if nx.is_connected(G.to_undirected()):
                    # If a cycle is created, remove the edge
                    G.remove_edge(i, j)

    return list(G.edges)

generate_acyclic_undirected_graph(10, 10)