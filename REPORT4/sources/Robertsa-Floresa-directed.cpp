#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <stdexcept>

using namespace std;

/*******************
  DIRECTED GRAPH
*******************/
bool hamiltonian(int vertex, vector<vector<int>>& graphMatrix, vector<int>& cycle, vector<bool>& visited, int n, int& vis, int start, int& k)
{
    visited[vertex - 1] = true;  // Add new vertex
    vis++;
    cycle.push_back(vertex);

    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] >= 0 && graphMatrix[vertex - 1][i] <= n)
        {
            if (i + 1 == start && vis == n)
            {
                cycle.push_back(i + 1);
                return true;
            }
            if (!visited[i])
            {
                if (hamiltonian(i + 1, graphMatrix, cycle, visited, n, vis, start, k))
                {
                    return true;
                }
            }
        }
    }
    visited[vertex - 1] = false;
    vis--;
    cycle.pop_back();
    return false;
}

bool hcycle(vector<vector<int>>& graphMatrix, vector<int>& cycle, vector<bool>& visited, int n)
{
    int k = 1;
    int vis = 0;
    int start = 1;
    return hamiltonian(start, graphMatrix, cycle, visited, n, vis, start, k);
}

unordered_map<int, vector<int>> generateSuccessorsList(const vector<pair<int, int>>& edges) {
    unordered_map<int, vector<int>> successors;
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        successors[u].push_back(v);
    }
    for (auto& pair : successors) {
        sort(pair.second.begin(), pair.second.end());
    }
    return successors;
}

unordered_map<int, vector<int>> generatePredecessorsList(const vector<pair<int, int>>& edges) {
    unordered_map<int, vector<int>> predecessors;
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        predecessors[v].push_back(u);
    }
    for (auto& pair : predecessors) {
        sort(pair.second.begin(), pair.second.end());
    }
    return predecessors;
}

unordered_map<int, vector<int>> generateIncidenceList(int vertices, const unordered_map<int, vector<int>>& predecessors, const unordered_map<int, vector<int>>& successors) {
    unordered_map<int, vector<int>> incidence;

    for (int i = 1; i <= vertices; ++i) {
        vector<int> incidentNodes;
        if (predecessors.find(i) != predecessors.end()) {
            incidentNodes.insert(incidentNodes.end(), predecessors.at(i).begin(), predecessors.at(i).end());
        }
        if (successors.find(i) != successors.end()) {
            incidentNodes.insert(incidentNodes.end(), successors.at(i).begin(), successors.at(i).end());
        }
        sort(incidentNodes.begin(), incidentNodes.end());
        incidence[i] = incidentNodes;
    }

    return incidence;
}

vector<vector<int>> fillMatrix(const unordered_map<int, vector<int>>& successors,
                               const unordered_map<int, vector<int>>& predecessors,
                               const unordered_map<int, vector<int>>& incidences,
                               int vertices) {

    // Initialize matrix
    vector<vector<int>> matrix(vertices, vector<int>(vertices + 3, 0));

    // Fill the rightmost columns based on Successors list
    for (const auto& pair : successors) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices] = pair.second[0];
        }
    }

    // Fill the rightmost columns based on Predecessors list
    for (const auto& pair : predecessors) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices + 1] = pair.second[0];
        }
    }

    // Fill the rightmost columns based on None Incidences list
    for (const auto& pair : incidences) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices + 2] = pair.second[0];
        }
    }

    // Fill the matrix based on Successors list
    for (const auto& pair : successors) {
        int vertex = pair.first;
        int row = vertex - 1;
        for (size_t i = 0; i < pair.second.size(); ++i) {
            int col = pair.second[i] - 1;
            matrix[row][col] = 1;
        }
    }

    // Fill the matrix based on Predecessors list
    for (const auto& pair : predecessors) {
        int vertex = pair.first;
        int row = vertex - 1;
        for (size_t i = 0; i < pair.second.size(); ++i) {
            int col = pair.second[i] - 1;
            matrix[row][col] = 1;
        }
    }

    // Ensure that all non-incident cells are marked properly
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (matrix[i][j] == 0) {
                matrix[i][j] = -1;
            }
        }
    }

    return matrix;
}

int main() {
    int vertices, edges;
    vector<pair<int, int>> edgesList;

    cin >> vertices >> edges;
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == 0 || v == 0) {
            cout << "0" << endl;
            return 0;
        }
        edgesList.push_back({ u, v });
    }

    unordered_map<int, vector<int>> successors = generateSuccessorsList(edgesList);
    unordered_map<int, vector<int>> predecessors = generatePredecessorsList(edgesList);
    unordered_map<int, vector<int>> incidences = generateIncidenceList(vertices, predecessors, successors);
    vector<vector<int>> graphMatrix = fillMatrix(successors, predecessors, incidences, vertices);

    vector<int> cycle;
    vector<bool> visited(vertices, false); // Vector to track visited vertices

    auto start = chrono::high_resolution_clock::now();
    if (hcycle(graphMatrix, cycle, visited, vertices)) {
        cout << "Hamiltonian cycle found: ";
        for (int vertex : cycle) {
            cout << vertex << " ";
        }
        cout << endl;
    } else {
        cout << "No Hamiltonian cycle found in the graph." << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time: " << duration.count() << " seconds" << endl;

    return 0;
}
