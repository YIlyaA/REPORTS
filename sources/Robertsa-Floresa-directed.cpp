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
bool hamiltionian(int vertex, vector<vector<int>>& graphMatrix, vector<int>& cycle, vector<bool>& visited, int n, int& vis, int start, int& k)
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
                if (hamiltionian(i + 1, graphMatrix, cycle, visited, n, vis, start, k))
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
    return hamiltionian(start, graphMatrix, cycle, visited, n, vis, start, k);
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

    // Sorting keys
    vector<int> sorted_keys;
    for (const auto& pair : successors) {
        sorted_keys.push_back(pair.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    // Creating sorted successors list
    unordered_map<int, vector<int>> sorted_successors;
    for (int key : sorted_keys) {
        sorted_successors[key] = successors[key];
    }

    return sorted_successors;
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

    // Sorting keys
    vector<int> sorted_keys;
    for (const auto& pair : predecessors) {
        sorted_keys.push_back(pair.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    // Creating sorted predecessors list
    unordered_map<int, vector<int>> sorted_predecessors;
    for (int key : sorted_keys) {
        sorted_predecessors[key] = predecessors[key];
    }

    return sorted_predecessors;
}

unordered_map<int, vector<int>> generateIncidenceList(int vertices, const unordered_map<int, vector<int>>& predecessors, const unordered_map<int, vector<int>>& successors) {
    unordered_map<int, vector<int>> incidence;
    unordered_map<int, vector<int>> none_incidence;

    vector<int> referenceVector;
    for (int i = 1; i <= vertices; ++i) {
        referenceVector.push_back(i);
    }

    // Iterating over each vertex
    for (const auto& pair : predecessors) {
        int vertex = pair.first;
        const vector<int>& pred = pair.second;

        // Adding predecessors
        incidence[vertex] = pred;

        // Adding successors
        if (successors.find(vertex) != successors.end()) {
            for (int succ : successors.at(vertex)) {
                incidence[vertex].push_back(succ);
            }
        }

        // Sorting the incidence list for this vertex
        sort(incidence[vertex].begin(), incidence[vertex].end());

        for (const auto& pair : incidence) {
            vector<int> missingElements;
            set_difference(
                referenceVector.begin(), referenceVector.end(),
                pair.second.begin(), pair.second.end(),
                back_inserter(missingElements)
            );

            if (!missingElements.empty()) {
                none_incidence[pair.first] = missingElements;
            }
        }
    }

    for (auto& pair : none_incidence) {
        sort(pair.second.begin(), pair.second.end());
    }

    // Sorting keys
    vector<int> sorted_keys;
    for (const auto& pair : none_incidence) {
        sorted_keys.push_back(pair.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    // Creating sorted none incidence list
    unordered_map<int, vector<int>> sorted_none_incidence;
    for (int key : sorted_keys) {
        sorted_none_incidence[key] = none_incidence[key];
    }

    return sorted_none_incidence;
}


vector<vector<int>> fillMatrix(unordered_map<int, vector<int>>& successors,
                               unordered_map<int, vector<int>>& predecessors,
                               unordered_map<int, vector<int>>& incidences,
                               int vertices) {

    // Initialize matrix
    vector<vector<int>> matrix(vertices, vector<int>(vertices + 3, 0));

    // Fill the rightmost columns based on Successors list
    for (auto& pair : successors) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices] = pair.second[0];
        }
    }

    // Fill the rightmost columns based on Predecessors list
    for (auto& pair : predecessors) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices + 1] = pair.second[0];
        }
    }

    // Fill the rightmost columns based on None Incidences list
    for (auto& pair : incidences) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices + 2] = pair.second[0];
        }
    }

    // Fill the matrix based on Successors list
    for (auto& pair : successors) {
        int vertex = pair.first;
        int row = vertex - 1;
        int counter = 0;
        for (int i = 0; i < pair.second.size(); ++i) {
            int col = pair.second[i] - 1;
            if (i + 1 >= pair.second.size()) matrix[row][col] = pair.second.back();  // If index is >= number of elements, insert the last element of the list
            else matrix[row][col] = pair.second[counter + 1];  // Insert the second element of the second part of the list, because the first is in the third column from the end
            counter += 1;
        }
    }

    // Fill the matrix based on Predecessors list
    for (auto& pair : predecessors) {
        int vertex = pair.first;
        int row = vertex - 1;
        int counter = 0;
        for (int i = 0; i < pair.second.size(); ++i) {
            int col = pair.second[i] - 1;
            if (i + 1 >= pair.second.size()) matrix[row][col] = pair.second.back() + vertices;
            else matrix[row][col] = pair.second[counter + 1] + vertices;
            counter += 1;
        }
    }

    // Fill the matrix based on None Incidences list
    for (auto& pair : incidences) {
        int vertex = pair.first;
        int row = vertex - 1;
        int counter = 0;
        for (int i = 0; i < vertices; ++i) {
            if (matrix[row][i] == 0) {
                if (i + 1 >= pair.second.size()) {
                    matrix[row][i] = -(pair.second.back());
                }
                else matrix[row][i] = -(pair.second[counter + 1]); // Add the last element with a minus sign from the None Incidences list
                counter += 1;
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
    }
    else {
        cout << "No Hamiltonian cycle found in the graph." << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time: " << duration.count() << " seconds" << endl;

    return 0;
}
