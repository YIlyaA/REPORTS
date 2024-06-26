#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

const int MAX_VERTICES = 200;

/*******************
  UNDIRECTED GRAPH
*******************/
class HamiltonianCycleFinder {
private:
    int vertices;
    int graph[MAX_VERTICES][MAX_VERTICES];
    vector<int> path;

public:
    HamiltonianCycleFinder(int v) {
        if (v > MAX_VERTICES) {
            throw runtime_error("Number of vertices exceeds the maximum limit.");
        }
        vertices = v;
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                graph[i][j] = 0;
            }
        }
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
            graph[u][v] = 1;
            graph[v][u] = 1;
        }
    }

    void printGraph() {
        cout << "Adjacency matrix:" << endl;
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool isSafe(int v, int pos) {
        if (graph[path[pos - 1]][v] == 0)
            return false;
        for (int i = 0; i < pos; ++i)
            if (path[i] == v)
                return false;
        return true;
    }

    bool hamiltonianCycleUtil(int pos) {
        if (pos == vertices) {
            if (graph[path[pos - 1]][path[0]] == 1)
                return true;
            else
                return false;
        }
        for (int v = 1; v < vertices; ++v) {
            if (isSafe(v, pos)) {
                path[pos] = v;
                if (hamiltonianCycleUtil(pos + 1))
                    return true;
                path[pos] = -1;
            }
        }
        return false;
    }

    bool hamiltonianCycle() {
        path.resize(vertices, -1);
        path[0] = 0;
        if (!hamiltonianCycleUtil(1)) {
            cout << "There is no Hamiltonian cycle in this graph." << endl;
            return false;
        }
        cout << "Found Hamiltonian cycle: ";
        for (int i = 0; i < vertices; ++i)
            cout << path[i] + 1 << " ";
        cout << path[0] + 1 << endl;
        return true;
    }
};

int main() {
    int vertices, edges;
    cin >> vertices >> edges;

    if (vertices > MAX_VERTICES) {
        cout << "The number of vertices exceeds the maximum allowed (" << MAX_VERTICES << ")." << endl;
        return 1;
    }

    HamiltonianCycleFinder hcFinder(vertices);
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == 0 || v == 0) {
            cout << "0" << endl;
            return 0;
        }
        hcFinder.addEdge(u - 1, v - 1);
    }

    auto start = chrono::high_resolution_clock::now();
    hcFinder.hamiltonianCycle();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time: " << duration.count() << " seconds" << endl;

    return 0;
}
