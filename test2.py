import networkx as nx
import random
import math

def generate_hamiltonian_cyclic_digraph(n, saturation):
    m = math.floor(saturation / 100 * (n * (n - 1)))

    # Ensure the graph is feasible
    if m < n or m > n * (n - 1):
        return []

    # Create an empty directed graph
    G = nx.DiGraph()

    # Add nodes (starting from 1 instead of 0)
    G.add_nodes_from(range(1, n + 1))

    # Create a Hamiltonian cycle
    hamiltonian_cycle = list(range(1, n + 1))
    random.shuffle(hamiltonian_cycle)
    cycle_edges = [(hamiltonian_cycle[i], hamiltonian_cycle[(i + 1) % n]) for i in range(n)]
    G.add_edges_from(cycle_edges)

    # List of all possible edges excluding the ones in the Hamiltonian cycle
    possible_edges = [(i, j) for i in range(1, n + 1) for j in range(1, n + 1) if
                      (i, j) not in G.edges() and i != j and (j, i) not in cycle_edges]

    # Randomly add edges until we reach the desired number of edges
    while len(G.edges) < m:
        if not possible_edges:
            # If no more possible edges are available, break the loop
            break
        edge = random.choice(possible_edges)
        G.add_edge(*edge)
        possible_edges.remove(edge)

    return list(G.edges)


print(generate_hamiltonian_cyclic_digraph(20, 10))
