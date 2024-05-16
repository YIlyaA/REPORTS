#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void fillMatrix(int** matrix, int n, int m, char inp)
{
    //sprawdzenie, czy nie ma zbyt wielu kraw�dzi
    if (m > n * (n - 1)) m = n * (n - 1);
    //dla ka�dej kraw�dzi: wprowad� par� liczb lub wylosuj, je�li cin zakomentowane
    //umie�� odpowiednio 1 i -1 w macierzy s�siedztwa
    if (inp == 'p') {
        string filename = "D:/PROGRAMMING/C/Graphs/input.txt"; // Имя файла, из которого читаем данные
        ifstream file(filename); // Создаем объект ifstream для чтения файла
        if (!file.is_open()) {
            cout << "Unable to open file" << filename << endl;
            exit(0);
        }
        int a, b;
        file >> a >> b;
        cout << a << " " << b;
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int vert1, vert2;
            if (iss >> vert1 >> vert2) {
                matrix[vert1][vert2] = 1;
                matrix[vert2][vert1] = -1;
            }
        }
        file.close();
    }
    else {
        cout << "Input edges:" << endl;
        for (int i = 0; i < m; i++)
        {
            int vert1, vert2;
            cin >> vert1 >> vert2;
            //sprawdzenie, czy wylosowane wierzcho�ki nie s� takie same
            if (vert1 == vert2) vert2 = n - 1;
            //            cout << vert1 << " " << vert2 << endl;
            matrix[vert1][vert2] = 1;
            matrix[vert2][vert1] = -1;
        }
    }
}

//wype�nienie listy nast�pnik�w na podstawie macierzy s�siedztwa
void fillList(int** matrix, list<list<int>>& nList, int n)
{
    //dla ka�dego wierzcho�ka
    for (int i = 0; i < n; i++) {
        //utw�rz list�
        list<int> vertList{ i };
        //wype�nij nast�pnikami z macierzy s�siedztwa
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) vertList.push_back(j);
        }
        //wypchnij list� do listy nast�pnik�w
        nList.push_back(vertList);
    }
}

//znalezienie wierzcho�ka o najmniejszym stopniu wej�ciowym na podstawie macierzy s�siedztwa
int getStartVertex(int** matrix, int n, int* colors)
{
    int vertex = -1;
    int value = n + 1;
    //dla ka�dego wierzcho�ka (wiersz w macierzy)
    for (int i = 0; i < n; i++)
    {
        //zlicz poprzedniki (-1 w wierszu macierzy)
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == -1) count++;
        }
        //je�li wierzcho�ek ma dot�d najmniejszy stopie� wej�ciowy i jest bia�y, wybierz go
        if (count < value && colors[i] == 0)
        {
            value = count;
            vertex = i;
        }
    }
    //zwr�� wierzcho�ek, lub -1 w przypadku problemu
    return vertex;
}

//znalezienie wierzcho�ka o najmniejszym stopniu wej�ciowym na podstawie listy nast�pnik�w
int getStartVertexList(list<list<int>>& nList, int n, int* colors)
{
    int vertex = -1;
    int value = n + 1;
    int count;
    //dla ka�dego wierzcho�ka
    for (int i = 0; i < n; i++)
    {
        count = 0;
        // dla ka�dego wierzcho�ka na ka�dej li�cie sprawd�, czy jest on na jakiej� li�cie nast�pnik�w (czyli ma poprzednika)
        for (auto& vertList : nList) {
            if (find(vertList.begin(), vertList.end(), i) != vertList.end()) {
                count++;
            }
        }
        //je�li wierzcho�ek ma dot�d najmniejszy stopie� wej�ciowy i jest bia�y, wybierz go
        if (count < value && colors[i] == 0)
        {
            value = count;
            vertex = i;
        }
    }
    //zwr�� wierzcho�ek, lub -1 w przypadku problemu
    return vertex;
}

//znajd� pierwszy bia�y nast�pnik wierzcho�ka w macierzy
int findWhiteSuccesor(int** matrix, int n, int vertex, int* colors)
{
    //dla ka�dego pola w wierszu, sprawd�, czy wierzcho�ek kolumny jest bia�y, zwr�� pierwszy natrafiony
    for (int j = 0; j < n; j++)
    {
        if (matrix[vertex][j] == 1 && colors[j] == 0) return j;
    }
    //zwr�� -1 w przypadku nieznalezienia
    return -1;
}

//znajd� pierwszy bia�y nast�pnik wierzcho�ka w li�cie
int findWhiteSuccesorList(list<list<int>>& nList, int n, int vertex, int* colors)
{
    //dla ka�dej podlisty
    for (auto& vertList : nList) {
        //je�li lista dotyczy szukanego wierzcho�ka (jest on jej g�ow�)
        if (vertList.front() == vertex)
        {
            //sprawd� nast�pniki, zwr�� pierwszy bia�y
            for (int num : vertList) {
                if (colors[num] == 0) return num;
            }
        }
    }
    //zwr�� -1 w przypadku nieznalezienia
    return -1;
}

//sprawd�, czy wierzcho�ek ma szare nast�pniki w macierzy
bool checkForGray(int** matrix, int n, int vertex, int* colors)
{
    //dla ka�dego pola w wierszu
    for (int j = 0; j < n; j++) {
        //je�li szary, zwr�� true
        if (matrix[vertex][j] == 1 && colors[j] == 1)
            return true;
    }
    //zwr�� false je�li nie ma szarych nast�pnik�w
    return false;
}

//sprawd�, czy ma szare nast�pniki w li�cie
bool checkForGrayList(list<list<int>>& nList, int n, int vertex, int* colors)
{
    //dla ka�dej pod listy
    for (auto& vertList : nList) {
        //je�li lista dotyczy szukanego wierzcho�ka (jest on jej g�ow�)
        if (vertList.front() == vertex)
        {
            //sprawd�, czy jest szary nast�pnik
            for (int num : vertList) {
                //je�li szary, zwr�� true
                if (colors[num] == 1 && num != vertex) return true;
            }
        }
    }
    //zwr�� false je�li nie ma szarych nast�pnik�w
    return false;
}

//tarjan_ms(macierz s�siedztwa, ilo�� wierzcho�k�w, lista kolor�w wierzcho�k�w, lista do kt�rej zwracamy posortowane, czy jest cykl)
void tarjanMatrix(int** matrix, int n, int vertex, int* colors, list<int>& sorted, bool& cycle)
{
    //ustaw wierzcho�ek na szary
    colors[vertex] = 1;
    //przerwij przed kolejnym wywo�aniem, je�li cykl
    if (cycle == 1) return;
    //sprawd� czy s� szare nast�pniki (mo�na od razy przenie�� checkForGray do if chyba, wcze�niej to inaczej troch� wygl�da�o)
    bool hasGray = checkForGray(matrix, n, vertex, colors);
    if (hasGray)
    {
        //przerwij je�li cykl (cykl istnieje, je�li sprawdzany wierzcho�ek ma szare nast�pniki)
        cycle = 1;
        return;
    }

    //znajd� pierwszego bia�ego nast�pnika
    int successor = findWhiteSuccesor(matrix, n, vertex, colors);

    /*cout << "GRAY" << endl;
    cout << vertex << endl;
    cout << successor << endl;
    cout << hasGray << endl;
    cout << "" << endl;*/

    //p�ki s� bia�e nast�pniki
    while (successor >= 0)
    {
        //wywo�aj rekurencyjnie (DFS) dla bia�ego nast�pnika
        tarjanMatrix(matrix, n, successor, colors, sorted, cycle);
        //znajd� kolejnego po powrocie z rekurencji
        successor = findWhiteSuccesor(matrix, n, vertex, colors);
    }
    //po powrocie z rekurencji, ustaw kolor na czarny
    colors[vertex] = 2;

    /*cout << "BLACK" << endl;
    cout << vertex << endl;
    cout << successor << endl;
    cout << hasGray << endl;
    cout << "" << endl;*/

    //umie�� wierzcho�ek z przodu listy
    sorted.push_front(vertex);
    return;
}

//tarjan_ls(lista nast�pnik�w, ilo�� wierzcho�k�w, lista kolor�w wierzcho�k�w, lista do kt�rej zwracamy posortowane, czy jest cykl)
//analogicznie jak powy�ej, r�nice w implementacji s� tylko w pomocniczych funkcjach
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

    /*cout << "GRAY" << endl;
    cout << vertex << endl;
    cout << successor << endl;
    cout << hasGray << endl;
    cout << "" << endl;*/

    while (successor >= 0)
    {
        tarjanList(nList, n, successor, colors, sorted, cycle);
        successor = findWhiteSuccesorList(nList, n, vertex, colors);
    }
    colors[vertex] = 2;

    /*cout << "BLACK" << endl;
    cout << vertex << endl;
    cout << successor << endl;
    cout << hasGray << endl;
    cout << "" << endl;*/

    sorted.push_front(vertex);
    return;
}

//znajdowanie niezale�nych wierzcho�k�w (deg_in == 0)
int findIndependent(int** matrix, int n, int* deleted)
{
    bool independent;
    //dla ka�dego wierzcho�ka
    for (int i = 0; i < n; i++)
    {
        independent = true;
        //dla ka�dego pola w wierzu
        for (int j = 0; j < n; j++)
        {
            //je�li ma -1 w polu, to ma poprzednika, czyli nie jest niezale�ny
            if (matrix[i][j] == -1) independent = false;
        }
        //je�li znaleziono niezale�ny i nie zosta� on wcze�niej usuni�ty, zwr�� wierzcho�ek
        if (independent && deleted[i] == 0)
        {
            return i;
        }
    }
    //zwr�� -1 w przypadku nieznalezienia
    return -1;
}

//znajd� niezale�ny w li�cie
int findIndependentList(list<list<int>>& nList, int n, int* deleted)
{
    int count;
    //dla ka�dego wierzcho�ka
    for (int i = 0; i < n; i++)
    {
        count = 0;
        //dla ka�dej podlisty
        for (auto& vertList : nList) {
            //je�li znajdzie wierzcho�ek w podli�cie, dodaje do ilo�ci poprzednik�w
            if (find(vertList.begin(), vertList.end(), i) != vertList.end()) {
                count++;
            }
        }
        //je�li znaleziono tylko raz (g�owa listy, kt�ra dotyczy tego wierzcho�ka)
        if (count == 1)
        {
            //zwr�� wierzcho�ek
            return i;
        }
    }
    //zwr�� -1 w przypadku nieznalezienia
    return -1;
}

//usuwanie wierzcho�ka z macierzy
void deleteVertex(int vertex, int** matrix, int n)
{
    //wyzeruj wiersz i kolumn� wierzcho�ka
    for (int i = 0; i < n; i++)
    {
        matrix[i][vertex] = 0;
        matrix[vertex][i] = 0;
    }
}

//usuwanie wierzcho�ka z listy
void deleteVertexList(int vertex, list<list<int>>& nList, int n)
{
    //sprawd� ka�d� list�, usu� list� kt�ra dotyczy tego wierzcho�ka, z ka�dej innej listy usu� ten wierzcho�ek je�li jest w nast�pnikach
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

//kahn_ms(macierz, ilo�� wierzcho�k�w, usuni�te wierzcho�ki, lista posortowana)
void kahnMS(int** matrix, int n, int* deleted, list<int>& sorted)
{
    //p�ki s� wierzcho�ki nieusuni�te
    while (sorted.size() < n)
    {
        //znajd� niezale�ny
        int vertex = findIndependent(matrix, n, deleted);
        //je�li znaleziono
        if (vertex != -1)
        {
            //dodaj na koniec listy posortowanej, usu� wierzcho�ek
            sorted.push_back(vertex);
            deleted[vertex] = 1;
            deleteVertex(vertex, matrix, n);
        }
            //je�li nie znaleziono, a nadal jest w while'u, to znaczy �e jest cykl
        else
        {
            cout << "Cykl. Sortowanie niemozliwe. (Kahn Matrix)" << endl;
            break;
        }
    }
}

//kahn_ln(lista nast�pnik�w, ilo�� wierzcho�k�w, usuni�te wierzcho�ki, lista posortowana)
//analogicznie jak powy�ej
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
    int n, m;
    int sV;
    char inp;
    cout << "Wczytac dane z pliku czy z klawiatury (p/k):";
    cin >> inp;
    if (inp == 'p') {
        string filename = "D:/PROGRAMMING/C/Graphs/input.txt";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Unable to open file " << filename << endl;
            return 1;
        }
        file >> n >> m;
        file.close();
    }
    else if (inp == 'k') {
        cout << "Podaj n i m:";
        cin >> n >> m;
        //        cout << n << " " << m << '\n';
    }
    else {
        cout << "ERROR";
        return 0;
    }


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

    fillMatrix(matrix, n, m, inp);
    fillList(matrix, nList, n);

    cout << endl;

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

    cout << "Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "List:" << endl;
    for (auto& vertList : nList) {
        for (int num : vertList) {
            cout << num << " ";
        }
        cout << endl;
    }

    cout << endl;

    cout << "Trajan z wierzcholka podanego przez uzytkownika (macierz)? (1 - tak, 0 - nie):";
    cin >> sV;

    while (sortedTrM.size() < n)
    {
        int startVertex = getStartVertex(matrix, n, sortColors);
        if (sV)
        {
            cout << "Podaj wierzcholek (macierz): " << endl;
            cin >> startVertex;
            sV = 0;
        }
        tarjanMatrix(matrix, n, startVertex, sortColors, sortedTrM, cycle);
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

    cout << "Trajan z wierzcholka podanego przez uzytkownika (lista)? (1 - tak, 0 - nie):";
    cin >> sV;

    while (sortedTrL.size() < n)
    {
        int startVertex = getStartVertexList(nList, n, sortColors);
        if (sV)
        {
            cout << "Podaj wierzcholek (lista): " << endl;
            cin >> startVertex;
            sV = 0;
        }
        tarjanList(nList, n, startVertex, sortColors, sortedTrL, cycle);
        if (cycle)
        {
            cout << "Cykl. Sortowanie niemozliwe. (Tarjan List)" << endl;
            break;
        }
    }

    kahnMS(kahnMatrix, n, deleted, sortedKnM);

    for (int i = 0; i < n; i++) {
        deleted[i] = 0;
    }

    kahnList(kahnNList, n, deleted, sortedKnL);

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


    if (!cycle)
    {
        cout << "Sorted by Tarjan with matrix:" << endl;
        for (int num : sortedTrM) {
            cout << num << " ";
        }

        cout << endl << endl;
        cout << "Sorted by Tarjan with list:" << endl;
        for (int num : sortedTrL) {
            cout << num << " ";
        }

        cout << endl << endl;
        cout << "Sorted by Kahn with matrix:" << endl;
        for (int num : sortedKnM) {
            cout << num << " ";
        }

        cout << endl << endl;
        cout << "Sorted by Kahn with list:" << endl;
        for (int num : sortedKnL) {
            cout << num << " ";
        }
        cout << endl;
    }
}