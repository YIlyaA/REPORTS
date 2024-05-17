#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;


//dfs po nieskierowanym
void dfsUD(int vertex, int** matrix, vector<bool>& visited, int n)
{
    visited[vertex] = true;
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1 && visited[i] == false) dfsUD(i, matrix, visited, n);
    }
}


//resetowanie tablicy odwiedzonych, przydaje sie do robienia dfsa wiele razy
void resetVisited(vector<bool>& visited, int n)
{
    for (int i = 0; i < n; i++) visited[i] = 0;
}


//dfs na nieskierowanym ze zliczaniem odwiedzonych, do sprawdzania mostow
int dfsUDCount(int vertex, int** matrix, vector<bool>& visited, int n, int excluded1, int excluded2, int& count)
{
    visited[vertex] = true;
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1 && visited[i] == false)
        {
            if (vertex != excluded1 || i != excluded2)
            {
                count++;
                dfsUDCount(i, matrix, visited, n, excluded1, excluded2, count);
            }
        }
    }
    return count;
}


//dfs na skierowanym ze zliczaniem, ostatecznie nieuzyty (zastapiony dfsFind, bo bylo pewniejsze)
int dfsDCount(int vertex, int** graphMatrix, vector<bool>& visited, int n, int excluded1, int excluded2, int& count, int** deletedMatrix)
{
    visited[vertex - 1] = true;
    if (graphMatrix[vertex - 1][n] != 0 && visited[graphMatrix[vertex - 1][n] - 1] == false)
    {
        if (vertex != excluded1 || graphMatrix[vertex - 1][n] != excluded2)
        {
            count++;
            if(deletedMatrix[vertex-1][graphMatrix[vertex - 1][n]-1] != 1) dfsDCount(graphMatrix[vertex - 1][n], graphMatrix, visited, n, excluded1, excluded2, count, deletedMatrix);
        }
    }
    if (graphMatrix[vertex - 1][n + 3] != 0 && visited[graphMatrix[vertex - 1][n + 3] - 1] == false)
    {
        if (vertex != excluded1 || graphMatrix[vertex - 1][n + 3] != excluded2)
        {
            count++;
            if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n + 3] - 1] != 1) dfsDCount(graphMatrix[vertex - 1][n + 3], graphMatrix, visited, n, excluded1, excluded2, count, deletedMatrix);
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] > 0 && graphMatrix[vertex - 1][i] <= n && visited[graphMatrix[vertex - 1][i] - 1] == false)
        {
            if (vertex != excluded1 || graphMatrix[vertex - 1][i] != excluded2)
            {
                count++;
                if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][i] - 1] != 1) dfsDCount(graphMatrix[vertex - 1][i], graphMatrix, visited, n, excluded1, excluded2, count, deletedMatrix);
            }
        }
    }
    return count;
}


//sprawdzanie, czy wierzcholek jest izolowany w nieskierowanym
bool isIsolatedUD(int vertex, int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1) return false;
    }
    return true;
}


//sprawdzanie, czy graf nieskierowany jest spojny (dfs z pierwszego nieizolowanego i sprawdzenie czy odwiedza wszystkie nieizolowane)
bool isConnectedUD(int** matrix, int n, vector<bool>& isolated, vector<bool>& visited)
{
    resetVisited(visited, n);
    for (int i = 0; i < n; i++)
    {
        if (isolated[i] == false)
        {
            dfsUD(i, matrix, visited, n);
            break;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (visited[i] == 0 && isolated[i] == 0)
        {
            return false;
        }
    }

    return true;
}


//wypelnianie macierzy sasiedztwa dla nieskierowanego
void fillMatrixUD(int** matrix, int n, int m)
{
    for (int i = 0; i < m; i++)
    {
        int vert1, vert2;
        cin >> vert1 >> vert2;
        //if (vert1 == vert2) vert2 = n - 1;
        matrix[vert1 - 1][vert2 - 1] = 1;
        matrix[vert2 - 1][vert1 - 1] = 1; //-1
    }
}


//sprawdzanie, czy w grafie nieskierowanym kazdy wierzcholek ma parzysty stopien
bool checkEvenUD(int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1) count++;
        }
        if (count % 2 != 0) return false;
    }
    return true;
}


//sprawdzenie mostu w nieskierowanym (dwa dfsy, jeden z wylaczona krawedzia dla ktorej jest sprawdzane, jesli ilosc odwiedzonych w jednym i drugim jest taka sama, to nie jest mostem)
bool checkBridgeUD(int vertex, int** matrix, vector<bool>& visited, int n, int edge)
{
    resetVisited(visited, n);
    int count0 = 0;
    dfsUDCount(vertex, matrix, visited, n, vertex, -1, count0);
    resetVisited(visited, n);
    int count1 = 0;
    dfsUDCount(vertex, matrix, visited, n, vertex, edge, count1);
    if (count0 != count1) return true;
    return false;
}


//sprawdzanie, czy mozna przejsc krawedzia w poszukiwaniu cyklu (mozna, jesli nie jest mostem, lub jesli jest to jedyna pozostala krawedz z wierzcholka)
bool checkValidUD(int vertex, int** matrix, vector<bool>& visited, int n, int edge)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1) count++;
    }
    if (count == 1)
    {
        return true;
    }

    if (!checkBridgeUD(vertex, matrix, visited, n, edge))
    {
        return true;
    }

    return false;
}


//poszukiwanie cyklu w nieskierowanym, algorym Fleury'ego
void findCycleUD(int vertex, int** matrix, vector<bool>& visited, int n, vector<int>& cycle)
{
    cycle.push_back(vertex);
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1)
        {
            if (checkValidUD(vertex, matrix, visited, n, i))
            {
                matrix[i][vertex] = 0;
                matrix[vertex][i] = 0;
                findCycleUD(i, matrix, visited, n, cycle);
            }
        }
    }
}


int main()
{
    int n, m;
    vector<int> cycle;
    bool isConnected;
    bool allEven;

    cin >> n >> m;

    int** matrix = new int* [n];

    for (int i = 0; i < n; i++)
    {
        matrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    vector<bool> visited(n, false);
    vector<bool> isolated(n, false);

    fillMatrixUD(matrix, n, m);

    int** matrixCopyJustInCase = new int* [n];

    for (int i = 0; i < n; i++)
    {
        matrixCopyJustInCase[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixCopyJustInCase[i][j] = matrix[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++)
    {
        if (isIsolatedUD(i, matrix, n)) isolated[i] = true;
    }

    isConnected = isConnectedUD(matrix, n, isolated, visited);
    allEven = checkEvenUD(matrix, n);

    if (isConnected && allEven)
    {
        int sV;
        for (int i = 0; i < n; i++)
        {
            if (isolated[i] == 0)
            {
                sV = i;
                break;
            }
        }
        findCycleUD(sV, matrix, visited, n, cycle);
    }
    else
    {
        cout << "Nie ma cyklu." << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time: " << duration.count() << " seconds" << endl;

    for (int i = 0; i < cycle.size(); ++i) {
        cout << cycle[i]+1 << " ";
    }
    cout << endl;

    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}