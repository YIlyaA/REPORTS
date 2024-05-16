#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

void fillMatrix(int** matrix, int n, int m)
{
    for (int i = 0; i < m; i++)
    {
        int vert1, vert2;
        cin >> vert1 >> vert2;
        if (vert1 == vert2) vert2 = n - 1;
        matrix[vert1][vert2] = 1;
        matrix[vert2][vert1] = -1;
    }
}

void fillList(int** matrix, list<list<int>>& nList, int n)
{
    for (int i = 0; i < n; i++) {
        list<int> vertList{ i };
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) vertList.push_back(j);
        }

        nList.push_back(vertList);
    }
}

int getStartVertex(int** matrix, int n, int* colors)
{
    int vertex = -1;
    int value = n + 1;
    for (int i = 0; i < n; i++)
    {
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == -1) count++;
        }
        if (count < value && colors[i] == 0)
        {
            value = count;
            vertex = i;
        }
    }
    return vertex;
}

int getStartVertexList(list<list<int>>& nList, int n, int* colors)
{
    int vertex = -1;
    int value = n + 1;
    int count;
    for (int i = 0; i < n; i++)
    {
        count = 0;
        for (auto& vertList : nList) {
            if (find(vertList.begin(), vertList.end(), i) != vertList.end()) {
                count++;
            }
        }

        if (count < value && colors[i] == 0)
        {
            value = count;
            vertex = i;
        }
    }
    return vertex;
}

int findWhiteSuccesor(int** matrix, int n, int vertex, int* colors)
{
    for (int j = 0; j < n; j++)
    {
        if (matrix[vertex][j] == 1 && colors[j] == 0) return j;
    }
    return -1;
}

int findWhiteSuccesorList(list<list<int>>& nList, int n, int vertex, int* colors)
{
    for (auto& vertList : nList) {
        if (vertList.front() == vertex)
        {
            for (int num : vertList) {
                if (colors[num] == 0) return num;
            }
        }
    }
    return -1;
}

bool checkForGray(int** matrix, int n, int vertex, int* colors)
{
    for (int j = 0; j < n; j++) {
        if (matrix[vertex][j] == 1 && colors[j] == 1)
            return true;
    }
    return false;
}

bool checkForGrayList(list<list<int>>& nList, int n, int vertex, int* colors)
{
    for (auto& vertList : nList) {
        if (vertList.front() == vertex)
        {
            for (int num : vertList) {
                if (colors[num] == 1 && num != vertex) return true;
            }
        }
    }
    return false;
}

void tarjanMatrix(int** matrix, int n, int vertex, int* colors, list<int>& sorted, bool& cycle)
{
    colors[vertex] = 1;
    if (cycle == 1) return;
    bool hasGray = checkForGray(matrix, n, vertex, colors);
    if (hasGray)
    {
        cycle = 1;
        return;
    }

    int successor = findWhiteSuccesor(matrix, n, vertex, colors);

    while (successor >= 0)
    {
        tarjanMatrix(matrix, n, successor, colors, sorted, cycle);
        successor = findWhiteSuccesor(matrix, n, vertex, colors);
    }
    colors[vertex] = 2;

    sorted.push_front(vertex);
    return;
}

void tarjanList(list<list<int>>& nList, int n, int vertex, int* colors, list<int>& sorted, bool& cycle)
{
    colors[vertex] = 1;
    if (cycle == 1) return;

    bool hasGray = checkForGrayList(nList, n, vertex, colors);
    if (hasGray)
    {
        cycle = 1;
        return;
    }

    int successor = findWhiteSuccesorList(nList, n, vertex, colors);
    while (successor >= 0)
    {
        tarjanList(nList, n, successor, colors, sorted, cycle);
        successor = findWhiteSuccesorList(nList, n, vertex, colors);
    }
    colors[vertex] = 2;
    sorted.push_front(vertex);
    return;
}

int findIndependent(int** matrix, int n, int* deleted)
{
    bool independent;
    for (int i = 0; i < n; i++)
    {
        independent = true;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == -1) independent = false;
        }
        if (independent && deleted[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

int findIndependentList(list<list<int>>& nList, int n, int* deleted)
{
    int count;
    for (int i = 0; i < n; i++)
    {
        count = 0;
        for (auto& vertList : nList) {
            if (find(vertList.begin(), vertList.end(), i) != vertList.end()) {
                count++;
            }
        }
        if (count == 1)
        {
            return i;
        }
    }
    return -1;
}

void deleteVertex(int vertex, int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        matrix[i][vertex] = 0;
        matrix[vertex][i] = 0;
    }
}

void deleteVertexList(int vertex, list<list<int>>& nList, int n)
{
    for (auto it = nList.begin(); it != nList.end(); ++it) {
        if (it->front() == vertex) {
            it = nList.erase(it);
            if (it == nList.end()) break;
        }
        else {
            it->remove(vertex);
        }
    }
}

void kahnMS(int** matrix, int n, int* deleted, list<int>& sorted)
{
    while (sorted.size() < n)
    {
        int vertex = findIndependent(matrix, n, deleted);
        if (vertex != -1)
        {
            sorted.push_back(vertex);
            deleted[vertex] = 1;
            deleteVertex(vertex, matrix, n);
        }
        else
        {
            cout << "Cykl. Sortowanie niemozliwe. (Kahn Matrix)" << endl;
            break;
        }
    }
}

void kahnList(list<list<int>>& nList, int n, int* deleted, list<int>& sorted)
{
    while (sorted.size() < n)
    {
        int vertex;
        if (nList.size() > 0) vertex = findIndependentList(nList, n, deleted);
        if (vertex != -1)
        {
            sorted.push_back(vertex);
            deleted[vertex] = 1;
            if (nList.size() > 0) deleteVertexList(vertex, nList, n);
        }
        else
        {
            cout << "Cykl. Sortowanie niemozliwe. (Kahn List)" << endl;
            break;
        }
    }
}


int main()
{
    const int size = 4;
    milliseconds::rep durations[size];

    int n, m;

    cin >> n >> m;

    int** matrix = new int* [n];
    int** kahnMatrix = new int* [n];
    list<list<int>> nList;

    list<int> sortedTrM;
    list<int> sortedTrL;
    list<int> sortedKnM;
    list<int> sortedKnL;

    bool cycle = false;

    for (int i = 0; i < n; i++)
    {
        matrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    fillMatrix(matrix, n, m);
    fillList(matrix, nList, n);

    list<list<int>> kahnNList;
    for (const auto& innerList : nList) {
        kahnNList.push_back(innerList);
    }

    for (int i = 0; i < n; i++) {
        kahnMatrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            kahnMatrix[i][j] = matrix[i][j];
        }
    }

    int* sortColors = new int[n];
    for (int i = 0; i < n; i++) {
        sortColors[i] = 0;
    }

    int* deleted = new int[n];
    for (int i = 0; i < n; i++) {
        deleted[i] = 0;
    }

    while (sortedTrM.size() < n)
    {
        auto start = high_resolution_clock::now();
        int startVertex = getStartVertex(matrix, n, sortColors);
        tarjanMatrix(matrix, n, startVertex, sortColors, sortedTrM, cycle);
        auto end = high_resolution_clock::now();
        auto duration1 = duration_cast<milliseconds>(end - start);
        durations[0] = duration1.count();
        if (cycle == 1)
        {
            cout << "Cykl. Sortowanie niemozliwe. (Tarjan Matrix)" << endl;
            break;
        }
    }

    cycle = false;
    for (int i = 0; i < n; i++) {
        sortColors[i] = 0;
    }

    while (sortedTrL.size() < n)
    {
        auto start = high_resolution_clock::now();
        int startVertex = getStartVertexList(nList, n, sortColors);
        auto end = high_resolution_clock::now();
        auto duration2 = duration_cast<milliseconds>(end - start);
        durations[1] = duration2.count();
        tarjanList(nList, n, startVertex, sortColors, sortedTrL, cycle);
        if (cycle)
        {
            cout << "Cykl. Sortowanie niemozliwe. (Tarjan List)" << endl;
            break;
        }
    }

    auto startt = high_resolution_clock::now();
    kahnMS(kahnMatrix, n, deleted, sortedKnM);
    auto endd = high_resolution_clock::now();
    auto duration3 = duration_cast<milliseconds>(endd - startt);
    durations[2] = duration3.count();

    for (int i = 0; i < n; i++) {
        deleted[i] = 0;
    }

    auto start = high_resolution_clock::now();
    kahnList(kahnNList, n, deleted, sortedKnL);
    auto end = high_resolution_clock::now();
    auto duration4 = duration_cast<milliseconds>(end - start);
    durations[3] = duration4.count();

    for (int i = 0; i < n; i++) {
        deleted[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        kahnMatrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            kahnMatrix[i][j] = matrix[i][j];
        }
    }

        cout << "duration (Tarjan with matrix): " << durations[0] << " ms" << endl;
        cout << "duration (Tarjan with list): " << durations[1] << " ms" << endl;
        cout << "duration (Kahn with matrix): " << durations[2] << " ms" << endl;
        cout << "duration (Kahn with list): " << durations[3] << " ms" << endl;
}