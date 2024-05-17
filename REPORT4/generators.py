import networkx as nx
import random
import math

def genRandomEulerS(n, sat):  # dla sąsiedztwa
    mat = [[0] * n for _ in range(n)]
    degsI = [0] * n
    degsO = [0] * n
    numEdges = n * (n - 1) * sat // 200

    startEdge = random.randrange(n)
    curEdge = startEdge
    i = 1
    errorCount = 0  # malutka szansa że nie zadziała dla super wysokiego nasycenia (zapędzi się w kozi róg) więc wtedy chcę wywalić
    while i < numEdges - sat // 10 or mat[startEdge][curEdge] == 1 or curEdge == startEdge:
        errorCount += 1
        newEdge = random.randrange(n)
        if errorCount > n * n * n / 2:
            return ["fail"]
        if newEdge == curEdge or mat[newEdge][curEdge] == 1 or mat[newEdge][curEdge] == -1 or degsI[newEdge] >= int(n) - 3 + (n % 2) or degsO[newEdge] >= int(n) - 3 + (n % 2):
            continue
        mat[newEdge][curEdge] = -1
        mat[curEdge][newEdge] = 1
        degsO[newEdge] += 1
        degsI[curEdge] += 1
        i += 1
        curEdge = newEdge

    mat[startEdge][curEdge] = -1
    mat[curEdge][startEdge] = 1
    degsO[startEdge] += 1
    degsI[curEdge] += 1

    imbalanced_vertices = []
    for v in range(n):
        if degsI[v] != degsO[v]:
            imbalanced_vertices.append(v)
            print(v)

    return mat


def convertMatrixToEdgeList(matrix):
    if matrix == ["fail"]:
        return "Failed to generate a valid Eulerian path/cycle.", []

    n = len(matrix)
    edges = []

    for i in range(n):
        for j in range(n):
            if matrix[i][j] == 1:
                edges.append((i, j))

    m = len(edges)

    result = [f"{n} {m}"]
    for edge in edges:
        result.append(f"{edge[0]+1} {edge[1]+1}")

    return "\n".join(result), edges

def generate_hamiltonian_cyclic_digraph(n, saturation):
    m = math.floor(saturation / 100 * (n * (n - 1)))

    # Ensure the graph is feasible
    if m < n or m > n * (n - 1):
        return "0 0"

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

def generate_undirected_hamiltonian_cyclic_graph(n, saturation):
    m = math.floor(saturation / 100 * (n * (n - 1)/2))

    # Ensure the graph is feasible
    if m < n or m > n * (n - 1):
        return "0 0"

    # Create an empty directed graph
    G = nx.Graph()

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

def generate_directed_euler_cyclic_graph(n, sat):  # dla sąsiedztwa
    mat = [[0] * n for _ in range(n)]
    degsI = [0] * n
    degsO = [0] * n
    numEdges = n * (n - 1) * sat // 100

    startEdge = random.randrange(n)
    curEdge = startEdge
    i = 1
    errorCount = 0  # malutka szansa że nie zadziała dla super wysokiego nasycenia (zapędzi się w kozi róg) więc wtedy chcę wywalić
    while i < numEdges - sat // 10 or mat[startEdge][curEdge] == 1 or curEdge == startEdge:
        errorCount += 1
        newEdge = random.randrange(n)
        if errorCount > n * n * n / 2:
            return ["fail"]
        if newEdge == curEdge or mat[newEdge][curEdge] == 1 or mat[newEdge][curEdge] == -1 or degsI[newEdge] >= int(n) - 3 + (n % 2) or degsO[newEdge] >= int(n) - 3 + (n % 2):
            continue
        mat[newEdge][curEdge] = -1
        mat[curEdge][newEdge] = 1
        degsO[newEdge] += 1
        degsI[curEdge] += 1
        i += 1
        curEdge = newEdge

    mat[startEdge][curEdge] = -1
    mat[curEdge][startEdge] = 1
    degsO[startEdge] += 1
    degsI[curEdge] += 1

    imbalanced_vertices = []
    for v in range(n):
        if degsI[v] != degsO[v]:
            imbalanced_vertices.append(v)
            print(v)

    edge_list_representation, edges = convertMatrixToEdgeList(mat)
    return edges


def generate_undirected_euler_cyclic_graph(n, sat):  # dla sąsiedztwa
    mat = [[0] * n for _ in range(n)]
    degs = [0] * n
    numEdges = n * (n - 1) * sat // 200

    startEdge = random.randrange(n)
    curEdge = startEdge
    i = 1
    errorCount = 0  # malutka szansa że nie zadziała dla super wysokiego nasycenia (zapędzi się w kozi róg) więc wtedy chcę wywalić
    while i < numEdges - sat // 10 or mat[startEdge][curEdge] == 1 or curEdge == startEdge:
        errorCount += 1
        newEdge = random.randrange(n)
        if errorCount > n * n * n / 2:
            return ["fail"]
        if newEdge == curEdge or mat[newEdge][curEdge] == 1 or degs[newEdge] >= n - 3 + (n % 2):
            continue
        mat[newEdge][curEdge] = 1
        mat[curEdge][newEdge] = 1
        degs[newEdge] += 1
        degs[curEdge] += 1
        i += 1
        curEdge = newEdge

    mat[startEdge][curEdge] = 1
    mat[curEdge][startEdge] = 1
    degs[startEdge] = 1
    degs[curEdge] = 1

    edge_list_representation, edges = convertMatrixToEdgeList(mat)
    return edges


def convertMatrixToEdgeList(matrix):
    if matrix == ["fail"]:
        return "Failed to generate a valid Eulerian path/cycle.", []

    n = len(matrix)
    edges = []

    for i in range(n):
        for j in range(n):
            if matrix[i][j] == 1:
                edges.append((i+1, j+1))

    m = len(edges)

    result = [f"{n} {m}"]
    for edge in edges:
        result.append(f"{edge[0]+1} {edge[1]+1}")

    return "\n".join(result), edges
