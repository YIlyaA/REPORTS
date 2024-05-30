#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
using namespace std;

void InsertionSort(vector<int> &data, int n, int gap)
{
    for (int i = gap; i < n; ++i)
    {
        int key = data[i];
        int j = i;

        while (j >= gap && data[j - gap] < key)
        {
            data[j] = data[j - gap];
            j -= gap;
        }
        data[j] = key;
    }
}

void ShellSort(vector<int> &data, int n)
{
    
    for (int gap = pow(2, floor(log2(n/2))) - 1; gap > 0; gap = (gap - 1) / 2)
    {
        //cout << "Gap: ";
        //cout << gap;
        //cout << endl;
        
        for (int i = gap; i < n; i++)
        {
            int key = data[i];
            int j = i;
            while (j >= gap && data[j - gap] < key)
            {
                data[j] = data[j - gap];
                j -= gap;
            }
            data[j] = key;
            /*for (int i = 0; i < n; i++) {
                cout << data[i] << " ";
            }
            cout << endl;*/
        }
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
    ShellSort(data, n);
    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }

    return 0;
}
