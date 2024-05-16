import networkx as nx
import random


def generate_directed_acyclic_graph(args):
    num_nodes, num_edges = args
    graph = nx.DiGraph()
    nodes = list(range(num_nodes))
    graph.add_nodes_from(nodes)

    # A set to keep track of added edges
    added_edges = set()

    # Generate edges until the graph becomes acyclic and reaches the desired number of edges
    while len(added_edges) < num_edges:
        node_from = random.choice(nodes)
        node_to = random.choice(nodes)

        if node_from != node_to and (node_from, node_to) not in added_edges:
            graph.add_edge(node_from, node_to)
            added_edges.add((node_from, node_to))

    return list(added_edges)


def generate_directed_cyclic_graph(args):
    num_nodes, num_edges = args
    graph = nx.DiGraph()
    nodes = list(range(num_nodes))
    graph.add_nodes_from(nodes)

    # A set to keep track of added edges
    added_edges = set()

    # Generate edges until the graph becomes cyclic and reaches the desired number of edges
    while len(added_edges) < num_edges:
        node_from = random.choice(nodes)
        node_to = random.choice(nodes)

        if node_from != node_to and (node_from, node_to) not in added_edges:
            # To create a cyclic graph, we allow adding edges that create cycles
            graph.add_edge(node_from, node_to)
            added_edges.add((node_from, node_to))


    return list(added_edges)


def generate_undirected_acyclic_graph(args):
    num_nodes, num_edges = args
    graph = nx.DiGraph()
    nodes = list(range(num_nodes))
    graph.add_nodes_from(nodes)

    edges_added = 0
    # progress_interval = num_edges // 100  # Определяем интервал обновления прогресса

    for _ in range(num_edges):
        node_from = random.choice(nodes)
        node_to = random.choice(nodes)

        if node_from != node_to:
            graph.add_edge(node_from, node_to)
            edges_added += 1
            # if edges_added % progress_interval == 0:
            #     tqdm.write(f"{desc}: {edges_added / num_edges * 100:.2f}% complete", end='\r')
            if not nx.is_directed_acyclic_graph(graph):
                graph.remove_edge(node_from, node_to)
                edges_added -= 1

    # tqdm.write(f"{desc}: 100.00% complete")
    return list(graph.edges())


def generate_undirected_cyclic_graph(args):
    num_nodes, num_edges = args
    graph = nx.Graph()
    nodes = list(range(num_nodes))
    graph.add_nodes_from(nodes)

    # Add all possible edges between nodes
    possible_edges = [(node1, node2) for node1 in nodes for node2 in nodes if node1 != node2]
    graph.add_edges_from(possible_edges)

    # Remove edges until the graph becomes cyclic and reaches the desired number of edges
    while graph.number_of_edges() > num_edges:
        # Choose a random edge and remove it
        edge_to_remove = random.choice(list(graph.edges()))
        graph.remove_edge(*edge_to_remove)

    return list(graph.edges())


# Example usage:
num_nodes = int(input())
# result = generate_directed_acyclic_graph((num_nodes, int(0.9 * num_nodes * (num_nodes - 1))))
# result1 = generate_undirected_acyclic_graph((num_nodes, int(0.9 * num_nodes * (num_nodes - 1) / 2)))
result2 = generate_directed_cyclic_graph((num_nodes, int(0.9 * num_nodes * (num_nodes - 1))))
result3 = generate_undirected_cyclic_graph((num_nodes, int(0.9 * num_nodes * (num_nodes - 1) / 2)))
print(num_nodes, int(0.9 * num_nodes * (num_nodes - 1)))
print(num_nodes, int(0.9 * num_nodes * (num_nodes - 1)/2))
# print("Generated DAG edges:\n", result)
# print("Generated UDAG edges:", result1)
print("Generated DCG edges:", result2)
print("Generated UDCG edges:", result3)
