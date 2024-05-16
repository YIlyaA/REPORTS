import math


def generate_directed_cyclic_graph(vertices, saturation):
    edges = math.floor(int(saturation / 100 * (vertices * (vertices - 1))))
    if edges < vertices:
        return 0
    edge_list = []
    for i in range(1, vertices):
        edge_list.append((i, i + 1))
    edge_list.append((vertices, 1))

    extra_edges = edges - vertices
    if extra_edges > 0:
        for i in range(1, vertices + 1):
            for j in range(1, vertices + 1):
                if extra_edges == 0:
                    break
                if i != j and (i, j) not in edge_list:
                    edge_list.append((i, j))
                    extra_edges -= 1
                    if extra_edges == 0:
                        break
    return edge_list


def generate_cyclic_undirected_graph(vertices, saturation):
    edges = math.floor(int(saturation / 100 * (vertices * (vertices - 1)) / 2))
    if edges < vertices:
        return "Impossible to genereate cyclic graph"
    edge_list = []
    for i in range(1, vertices):
        edge_list.append((i, i + 1))
    edge_list.append((vertices, 1))

    extra_edges = edges - vertices
    if extra_edges > 0:
        for i in range(1, vertices + 1):
            for j in range(i + 2, vertices + 1):
                if extra_edges == 0:
                    break
                if (i, j) not in edge_list and (j, i) not in edge_list:
                    edge_list.append((i, j))
                    extra_edges -= 1
                    if extra_edges == 0:
                        break
    return edge_list


# n = int(input())
# edge_factor1 = 10
# print(n, edge_factor1)
# edges = generate_cyclic_undirected_graph(n, edge_factor1)
# result = '\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in edges)
# print("Krawedzi:", result)
#
# edge_factor2 = 10
# edges2 = generate_directed_cyclic_graph(n, edge_factor2)
# if edges2 == 0:
#     print("Impossible to generate cyclic graph")
# else:
#     result1 = '\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in edges2)
#     print("Krawedzi:")
#     print(result1)
