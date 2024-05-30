#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

int height(struct Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int maximum(int a, int b) {
    return (a > b) ? a : b;
}

Node* insert(Node* node, int key) {
    if (node == NULL) return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

Node* minValueNode(Node* node) {
    struct Node* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

void bubbleSort(int arr[], int size) {
    int i, j, temp;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


Node* constructAVL(int numbers[], int begin, int end, Node* root, int height)
{
    if (begin > end)
    {
        return root;
    }
    int mid = (begin + end) / 2;
    //printf("%d\n", mid);

    root = insert(root, numbers[mid]);

    root = constructAVL(numbers, begin, mid - 1, root, height + 1);
    root = constructAVL(numbers, mid + 1, end, root, height + 1);

}


#ifdef _WIN32
#include <Windows.h>
double what_time_is_it()
{
    FILETIME ft;
    ULONGLONG nTime100;
    GetSystemTimeAsFileTime(&ft);
    nTime100 = ((ULONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
    return (double)nTime100 * 1e-9;
}
#else
#include <time.h>
double what_time_is_it()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec * 1e-9;
}
#endif

int main() {
    struct Node* root = NULL;
    int n, key;
    double start, end, create_time, find_min_time, balance_time;

    scanf("%d", &n);
    int* numbers = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        scanf("%d", &key);
        numbers[i] = key;

    }
    /*
    for (int i = 0; i < n; ++i) {
        key = rand() % 100 + 1;
        numbers[i] = key;
        printf("%d ", key);
    }*/

    bubbleSort(numbers, n);


    start = what_time_is_it();
    root = constructAVL(numbers, 0, n - 1, root, 0);
    end = what_time_is_it();
    create_time = end - start;

    start = what_time_is_it();
    Node* minNode = minValueNode(root);
    //printf("min: %d\n", minNode->key);
    end = what_time_is_it();
    find_min_time = end - start;

    
    printf("create_time: %.20f\n", create_time);
    printf("find_min_time: %.20f\n", find_min_time);

    return 0;
}
