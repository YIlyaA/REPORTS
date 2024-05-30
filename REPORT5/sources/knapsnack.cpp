#include <iostream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;

string toBinary(int x, int num_bits) {
    //    int num_bits = log2(x) + 1;
    bitset<256> binary(x);
    string binary_str = binary.to_string();
    return binary_str.substr(256 - num_bits, num_bits);
}


int bruteForce(int c, int n, int* masses, int* values, int* ids, vector<int>& stateSave, int& massSave)
{
    int mass;
    int value;
    int max = 0;
    string state;
    for (int i = 1; i <= (pow(2, n) - 1); i++)
    {
        state = toBinary(i, n);
        mass = 0;
        value = 0;
        //        cout << state << endl;
        for (int j = 0; j < n; j++)
        {
            if (state[j] == '1')
            {
                mass += masses[j];
                value += values[j];
            }
        }
        if (mass <= c && value >= max)
        {
            max = value;
            massSave = mass;
            stateSave.clear();
            for (int j = 0; j < n; j++) if (state[j] == '1') stateSave.push_back(ids[j]);
        }
    }
    return max;
}

void sortByVM(int* values, int* masses, int* ids, int n) {
    vector<int> indices(n);
    for (int i = 0; i < n; ++i) {
        indices[i] = i;
    }
    sort(indices.begin(), indices.end(), [values, masses](int i, int j) {
        return (static_cast<double>(values[i]) / masses[i]) > (static_cast<double>(values[j]) / masses[j]);
    });

    vector<int> values_sorted(n);
    vector<int> masses_sorted(n);
    vector<int> ids_sorted(n);

    for (int i = 0; i < n; ++i) {
        values_sorted[i] = values[indices[i]];
        masses_sorted[i] = masses[indices[i]];
        ids_sorted[i] = ids[indices[i]];
    }

    for (int i = 0; i < n; ++i) {
        values[i] = values_sorted[i];
        masses[i] = masses_sorted[i];
        ids[i] = ids_sorted[i];
    }
}

int greedy(int c, int n, int* masses, int* values, int* ids, vector<int>& stateSave, int& massSave) {
    sortByVM(values, masses, ids, n);

    int load = 0;
    int i = 0;
    int max = 0;
    while (load <= c && i < n) {
        if (load + masses[i] <= c) {
            stateSave.push_back(ids[i]);
            max += values[i];
            load += masses[i];
        }
        i++;
    }
    massSave = load;
    return max;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int bellman(int i, int j, int** matrix, int* masses, int* values, int c) {
    if (i == 0 || j == 0) return 0;

    if (masses[i - 1] > j) return bellman(i - 1, j, matrix, masses, values, c);

    return max(bellman(i - 1, j, matrix, masses, values, c), bellman(i - 1, j - masses[i - 1], matrix, masses, values, c) + values[i - 1]);
}

void getSolution(int i, int j, int** matrix, int* masses, int* values, int* ids, int c, vector<int>& state, int& massSave) {
    if (i == 0) return;
    if (matrix[i][j] > matrix[i - 1][j]) {
        state.push_back(ids[i - 1]);
        massSave += masses[i - 1];
        getSolution(i - 1, j - masses[i - 1], matrix, masses, values, ids, c, state, massSave);
    } else {
        getSolution(i - 1, j, matrix, masses, values, ids, c, state, massSave);
    }
}

int main() {
    int c, n;
    cin >> n >> c;
    if (cin.fail() || n <= 0 || c <= 0) {
        cerr << "Bledne dane, oczekiwano liczb naturalnych." << endl;
        return 1;
    }

    int* masses = new int[n];
    int* values = new int[n];
    int* ids = new int[n];
    int mass = 0;
    vector<int> state;

    for (int i = 0; i < n; i++) {
        cin >> masses[i] >> values[i];
        if (cin.fail() || masses[i] <= 0 || values[i] <= 0) {
            cerr << "Bledne dane, oczekiwano liczb naturalnych." << endl;
            delete[] masses;
            delete[] values;
            delete[] ids;
            return 1;
        }
    }

    for (int i = 0; i < n; i++) ids[i] = i + 1;

    auto start = chrono::high_resolution_clock::now();
    cout << "Brute force: ";
    cout << bruteForce(c, n, masses, values, ids, state, mass);
    cout << " " << mass << endl;
    cout << "ID wybranych elementow: ";
    for (int id : state) {
        cout << id << " ";
    }
    cout << endl;
    state.clear();
    mass = 0;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time for brute_force: " << duration.count() << " seconds" << endl;
    cout << endl;

    int** matrix = new int*[n + 1];
    for (int i = 0; i <= n; ++i) {
        matrix[i] = new int[c + 1];
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= c; j++) {
            matrix[i][j] = 0;
        }
    }

    auto start1 = chrono::high_resolution_clock::now();
    cout << "Dynamiczny: ";
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= c; j++) {
            if (masses[i - 1] > j) {
                matrix[i][j] = matrix[i - 1][j];
            } else {
                matrix[i][j] = max(matrix[i - 1][j], matrix[i - 1][j - masses[i - 1]] + values[i - 1]);
            }
        }
    }
    cout << matrix[n][c];
    getSolution(n, c, matrix, masses, values, ids, c, state, mass);
    cout << " " << mass << endl;
    cout << "ID wybranych elementow: ";
    for (int id : state) {
        cout << id << " ";
    }
    cout << endl;
    state.clear();
    mass = 0;
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration1 = end1 - start1;
    cout << "Time for dynamic: " << duration1.count() << " seconds" << endl;
    cout << endl;

    auto start2 = chrono::high_resolution_clock::now();
    cout << "Zachlanny: ";
    cout << greedy(c, n, masses, values, ids, state, mass);
    cout << " " << mass << endl;
    cout << "ID wybranych elementow: ";
    for (int id : state) {
        cout << id << " ";
    }
    cout << endl;
    state.clear();
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration2 = end2 - start2;
    cout << "Time for greedy: " << duration2.count() << " seconds" << endl;

    delete[] masses;
    delete[] values;
    delete[] ids;
    for (int i = 0; i <= n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
