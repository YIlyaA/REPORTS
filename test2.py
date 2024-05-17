import networkx as nx
import random
import math

def genRandomEulerS(n, sat):  # dla sąsiedztwa
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


mat = genRandomEulerS(20, 10)
print(mat)
