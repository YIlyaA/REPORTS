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


//dfs po skierowanym
void dfsD(int vertex, int** graphMatrix, vector<bool>& visited, int n, vector<vector<int>>& cycles)
{
    visited[vertex - 1] = true;
    //pierwszy nastepnik, w sumie bez sensu troche
    if (graphMatrix[vertex - 1][n] != 0 && visited[graphMatrix[vertex - 1][n] - 1] == false)
    {
        dfsD(graphMatrix[vertex - 1][n], graphMatrix, visited, n, cycles);
    }
    //petle
    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (visited[cycles[vertex - 1][i] - 1] == false)
        {
            dfsD(cycles[vertex - 1][i], graphMatrix, visited, n, cycles);
        }
    }
    /*if (graphMatrix[vertex - 1][n + 3] != 0 && visited[graphMatrix[vertex - 1][n + 3] - 1] == false)
    {
        dfsD(graphMatrix[vertex - 1][n + 3], graphMatrix, visited, n, cycles);
    }*/
    //nastepniki z pol macierzy
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] > 0 && graphMatrix[vertex - 1][i] <= n && visited[graphMatrix[vertex - 1][i] - 1] == false)
        {
            dfsD(graphMatrix[vertex - 1][i], graphMatrix, visited, n, cycles);
        }
    }
}


//resetowanie tablicy odwiedzonych, przydaje sie do robienia dfsa wiele razy
void resetVisited(vector<bool>& visited, int n)
{
    for (int i = 0; i < n; i++) visited[i] = 0;
}


//tworzenie listy nastepnikow z macierzy sasiedztwa
void getSuccessors(int** matrix, vector<vector<int>>& successors, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        successors.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                successors[i].push_back(j + 1);
            }
        }
    }
}


//tworzenie listy poprzednikow z macierzy sasiedztwa
void getPredecessors(int** matrix, vector<vector<int>>& predecessors, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        predecessors.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == -1)
            {
                predecessors[i].push_back(j + 1);
            }
        }
    }
}


//tworzenie listy nieincydentnych z macierzy sasiedztwa
void getNonIncidents(int** matrix, vector<vector<int>>& nonIncidents, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        nonIncidents.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                nonIncidents[i].push_back(j + 1);
            }
        }
    }
}


//tworzenie listy petli z macierzy sasiedztwa
void getCycles(int** matrix, vector<vector<int>>& cycles, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        cycles.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 2 || matrix[j][i] == 2) cycles[i].push_back(j + 1);
        }
    }
}


//dfs na skierowanym szukajacy danego wierzcholka, do sprawdzania mostow na skierowanym
void dfsDFind(int vertex, int** graphMatrix, vector<bool>& visited, int n, int target, int** deletedMatrix, bool& found, vector<vector<int>>& cycles)
{
    visited[vertex - 1] = true;
    if (vertex == target) found = true;
    if (graphMatrix[vertex - 1][n] != 0 && visited[graphMatrix[vertex - 1][n] - 1] == false)
    {
        if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n] - 1] != 1)
        {
            //if (graphMatrix[vertex - 1][n] == target) return true;
            dfsDFind(graphMatrix[vertex - 1][n], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
        }
    }
    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (visited[cycles[vertex - 1][i] - 1] == false && deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] != 1)
        {
            dfsDFind(cycles[vertex - 1][i], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] > 0 && graphMatrix[vertex - 1][i] <= n && visited[graphMatrix[vertex - 1][i] - 1] == false)
        {
            if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][i] - 1] != 1)
            {
                //if (graphMatrix[vertex - 1][i] == target) return true;
                dfsDFind(graphMatrix[vertex - 1][i], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
            }
        }
    }
}


//sprawdzanie, czy jest izolowany na skierowanym
bool isIsolatedD(int vertex, int** graphMatrix, int n)
{
    if (graphMatrix[vertex - 1][n] == 0 && graphMatrix[vertex - 1][n + 1] == 0 && graphMatrix[vertex - 1][n + 3] == 0) return true;
    return false;
}


//sprawdzenie, czy graf skierowany jest spojny (dfs, dfs na odwroconych lukach, sprawdzenie czy gdzies jest nieodwiedzony w obu i nieizolowany, algorytm z geeks for geeks)
bool isConnectedD(int** graphMatrix, int** graphMatrixInv, int n, vector<bool>& isolated, vector<bool>& visited, vector<bool>& visitedInv, vector<vector<int>>& cycles)
{
    resetVisited(visited, n);
    for (int i = 0; i < n; i++)
    {
        if (isolated[i] == false)
        {
            dfsD(i + 1, graphMatrix, visited, n, cycles);
            dfsD(i + 1, graphMatrixInv, visitedInv, n, cycles);
            break;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (visited[i] == 0 && visitedInv[i] == 0 && isolated[i] == 0)
        {
            return false;
        }
    }

    return true;
}


//wypelnianie macierzy sasiedztwa dla skierowanego, 2 oznacza petle
void fillMatrixD(int** matrix, int n, int m)
{
    for (int i = 0; i < m; i++)
    {
        int vert1, vert2;
        cin >> vert1 >> vert2;
        if (vert1 == vert2)
        {
            matrix[vert1 - 1][vert2 - 1] = 2;
        }
        else if (matrix[vert2 - 1][vert1 - 1] == 1)
        {
            matrix[vert1 - 1][vert2 - 1] = 2;
            matrix[vert2 - 1][vert1 - 1] = 2;
        }
        else
        {
            matrix[vert1 - 1][vert2 - 1] = 1;
            matrix[vert2 - 1][vert1 - 1] = -1; //-1
        }
    }
}


//sprawdzanie, czy w grafie skierowanym dla kazdego wierzcholka |in| == |out|
bool checkInOutD(int** graphMatrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        int countS = 0;
        int countP = 0;
        for (int j = 0; j < n; j++)
        {
            if (graphMatrix[i][j] >= 0 && graphMatrix[i][j] <= n) countS++;
        }
        for (int j = 0; j < n; j++)
        {
            if (graphMatrix[i][j] >= n + 1 && graphMatrix[i][j] <= 2 * n) countP++;
        }
        if (countS != countP) return false;
    }
    return true;
}


//sprawdzanie mostu w grafie skierowanym (puszczanie dfsa z konca luku, jesli dfs znajdzie poczatek luku, to nie jest mostem)
bool checkBridgeD(int vertex, int** graphMatrix, vector<bool>& visited, int n, int edge, int** deletedMatrix, vector<vector<int>>& cycles)
{
    resetVisited(visited, n);
    bool found = false;
    dfsDFind(edge, graphMatrix, visited, n, vertex, deletedMatrix, found, cycles);
    if (found)
    {
        return false;
    }
    return true;
}


//sprawdzanie, czy mozna przejsc lukiem w poszukiwaniu cyklu (mozna, jesli nie jest mostem, lub jesli jest to jedyny pozostaly luk wychodzacy z wierzcholka)
bool checkValidD(int vertex, int** graphMatrix, vector<bool>& visited, int n, int edge, int** deletedMatrix, vector<vector<int>>& cycles)
{
    int count = 0;

    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] != 1)
        {
            count++;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] >= 0 && graphMatrix[vertex - 1][i] <= n && deletedMatrix[vertex - 1][i] != 1)
        {
            count++;
        }
    }
    if (count == 1)
    {
        return true;
    }

    if (!checkBridgeD(vertex, graphMatrix, visited, n, edge, deletedMatrix, cycles))
    {
        return true;
    }

    return false;
}


//poszukiwanie cyklu w skierowanym, algorym Fleury'ego,
void findCycleD(int vertex, int** graphMatrix, vector<bool>& visited, int n, vector<int>& cycle, int** deletedMatrix, vector<vector<int>>& cycles)
{
    cycle.push_back(vertex);
    //pierwszy nastepnik, w sumie bez sensu
    if (graphMatrix[vertex - 1][n] != 0 && deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n] - 1] != 1)
    {
        if (checkValidD(vertex, graphMatrix, visited, n, graphMatrix[vertex - 1][n], deletedMatrix, cycles))
        {
            deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n] - 1] = 1;
            findCycleD(graphMatrix[vertex - 1][n], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
        }
    }

    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] != 1)
        {
            if (checkValidD(vertex, graphMatrix, visited, n, cycles[vertex - 1][i], deletedMatrix, cycles))
            {
                deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] = 1;
                findCycleD(cycles[vertex - 1][i], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
            }
        }
    }
    //kolejne nastepniki
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] >= 0 && graphMatrix[vertex - 1][i] <= n && deletedMatrix[vertex - 1][graphMatrix[vertex - 1][i] - 1] != 1)
        {
            if (checkValidD(vertex, graphMatrix, visited, n, graphMatrix[vertex - 1][i], deletedMatrix, cycles))
            {
                deletedMatrix[vertex - 1][graphMatrix[vertex - 1][i] - 1] = 1;
                findCycleD(graphMatrix[vertex - 1][i], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
            }
        }
    }
}

//odwracanie macierzy, zeby moc obrocic luki w grafie skierowanym
void inverseMatrix(int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                matrix[i][j] = -1;
            }
            if (matrix[i][j] == -1)
            {
                matrix[i][j] = 1;
            }
        }
    }
}


//wypelnianie macierzy grafu, algorytm z ekursow
void fillGraphMatrix(int** graphMatrix, int** matrix, int n, vector<vector<int>>& successors, vector<vector<int>>& predecessors, vector<vector<int>>& nonIncident, vector<vector<int>>& cycles)
{
    //Krok 1 :

    for (int i = 0; i < n; i++)
    {
        if (successors[i].size() > 1)
        {
            graphMatrix[i][n] = successors[i][1];
        }
        else graphMatrix[i][n] = 0;
    }

    int count;
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (successors[i].size() == 1)
            {
                graphMatrix[i][j] = 0;
            }
            else
            {
                if (matrix[i][j] == 1)
                {
                    if (successors[i].size() > count)
                    {
                        graphMatrix[i][j] = successors[i][count];
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = successors[i][successors[i].size() - 1];
                    }
                }
            }
        }
    }

    //Krok 2:

    for (int i = 0; i < n; i++)
    {
        if (predecessors[i].size() > 1)
        {
            graphMatrix[i][n + 1] = predecessors[i][1];
        }
        else graphMatrix[i][n + 1] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (matrix[j][i] == 1)
            {
                if (predecessors[i].size() == 1)
                {
                    graphMatrix[i][j] = 0;
                }
                else
                {
                    if (predecessors[i].size() > count)
                    {
                        graphMatrix[i][j] = predecessors[i][count] + n;
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = predecessors[i][predecessors[i].size() - 1] + n;
                    }
                }
            }
        }
    }

    //Krok 3:

    for (int i = 0; i < n; i++)
    {
        if (nonIncident[i].size() > 1)
        {
            graphMatrix[i][n + 2] = nonIncident[i][1];
        }
        else graphMatrix[i][n + 2] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                if (nonIncident[i].size() == 1)
                {
                    graphMatrix[i][j] = 0;
                }
                else
                {
                    if (nonIncident[i].size() > count)
                    {
                        graphMatrix[i][j] = -nonIncident[i][count];
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = -nonIncident[i][nonIncident[i].size() - 1];
                    }
                }
            }
        }
    }

    //Krok 4:

    for (int i = 0; i < n; i++)
    {
        if (cycles[i].size() > 1)
        {
            graphMatrix[i][n + 3] = cycles[i][1];
        }
        else graphMatrix[i][n + 3] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (cycles[i].size() != 1)
            {
                if (matrix[i][j] == 2)
                {
                    if (cycles[i].size() > count)
                    {
                        graphMatrix[i][j] = cycles[i][count] + 2 * n;
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = cycles[i][cycles[i].size() - 1] + 2 * n;
                    }
                }
            }
        }
    }
}


int main()
{
    int n, m;
    vector<int> cycle;
    bool isConnected;

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
    vector<bool> visitedInv(n, false);
    vector<bool> isolated(n, false);

    vector<vector<int>> successors;
    vector<vector<int>> predecessors;
    vector<vector<int>> nonIncident;
    vector<vector<int>> cycles;

    int** graphMatrix = new int* [n];
    for (int i = 0; i < n; i++)
    {
        graphMatrix[i] = new int[n + 4];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 4; j++) {
            graphMatrix[i][j] = 0;
        }
    }

    int** graphMatrixInv = new int* [n];
    for (int i = 0; i < n; i++)
    {
        graphMatrixInv[i] = new int[n + 4];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 4; j++) {
            graphMatrixInv[i][j] = 0;
        }
    }

    fillMatrixD(matrix, n, m);

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

    int** deletedMatrix = new int* [n];

    for (int i = 0; i < n; i++)
    {
        deletedMatrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            deletedMatrix[i][j] = 0;
        }
    }

    getSuccessors(matrix, successors, n);
    getPredecessors(matrix, predecessors, n);
    getNonIncidents(matrix, nonIncident, n);
    getCycles(matrix, cycles, n);

    fillGraphMatrix(graphMatrix, matrix, n, successors, predecessors, nonIncident, cycles);
    inverseMatrix(matrix, n);
    fillGraphMatrix(graphMatrixInv, matrix, n, successors, predecessors, nonIncident, cycles);
    inverseMatrix(matrix, n);

    auto start = chrono::high_resolution_clock::now();

    resetVisited(visited, n);
    dfsD(1, graphMatrix, visited, n, cycles);

    for (int i = 0; i < n; i++)
    {
        if (isIsolatedD(i + 1, graphMatrix, n)) isolated[i] = true;
    }


    isConnected = isConnectedD(graphMatrix, graphMatrixInv, n, isolated, visited, visitedInv, cycles);
    bool isEqualInOut = checkInOutD(graphMatrix, n);

    if (isConnected && isEqualInOut)
    {
        int sV = 0;
        for (int i = 0; i < n; i++)
        {
            if (isolated[i] == 0)
            {
                sV = i + 1;
                break;
            }
        }
        findCycleD(sV, graphMatrix, visited, n, cycle, deletedMatrix, cycles);

        for (int i = 0; i < cycle.size(); ++i) {
            cout << cycle[i] << " ";
        }
        cout << endl;
    }
    else
    {
        cout << "Nie ma cyklu." << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time: " << duration.count() << " seconds" << endl;

    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    for (int i = 0; i < n; ++i) {
        delete[] graphMatrix[i];
    }
    delete[] graphMatrix;

    for (int i = 0; i < n; ++i) {
        delete[] graphMatrixInv[i];
    }
    delete[] graphMatrixInv;

    for (int i = 0; i < n; ++i) {
        delete[] matrixCopyJustInCase[i];
    }
    delete[] matrixCopyJustInCase;

    for (int i = 0; i < n; ++i) {
        delete[] deletedMatrix[i];
    }
    delete[] deletedMatrix;

    return 0;
}