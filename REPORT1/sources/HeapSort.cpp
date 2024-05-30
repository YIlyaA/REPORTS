#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
using namespace std;

void heapify(vector<int> &data, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && data[largest] > data[left])
    {
        largest = left;
    }

    if (right < n && data[largest] > data[right])
    {
        largest = right;
    }

    if (largest != i)
    {
        swap(data[i], data[largest]);

        heapify(data, n, largest);
    }
}

void sort(vector<int> &data, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(data, n, i);
    }
    for (int i = n - 1; i > 0; i--)
    {
        swap(data[0], data[i]);  
        heapify(data, i, 0);
    }
}

int main() 
{
    vector<int> data;
    string input;
    string toggle;

    cin >> toggle;
    cin.ignore();
        while (getline(cin, input))
        {
            if (input == "") break;
            int number;
            stringstream ss(input);
            while (ss >> number) {
               data.push_back(number);
            }
        }
    

    int n = data.size();
    sort(data, n);
    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }
 

    return 0;
}
