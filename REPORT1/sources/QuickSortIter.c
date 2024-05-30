#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];                      
    int i = low - 1;                            

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;  
            swap(&arr[i], &arr[j]); 
        }
    }
    swap(&arr[i + 1], &arr[high]);  
    return i + 1; 
}

void QuickSort(int arr[], int low, int high) {
    int stack[high - low + 1]; 
    int top = -1; 

    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        high = stack[top--];  
        low = stack[top--]; 

        int pi = partition(arr, low, high);

        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }
        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }
}

int main() {
    int n, num;
    scanf("%d", &n);

    int arr[n];

    /* generator losowych liczb
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        num = rand() % 100;
        printf("%d ", num);
        arr[i] = num;
    }
    printf("\n");
    */

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    QuickSort(arr, 0, n - 1);

    for (int i = n-1; i >= 0; i--) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
