#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

Node* newNode(int item) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->key = item;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

Node* insert(Node* node, int key) {
    if (node == NULL) return newNode(key);
    if (key < node->key) node->left = insert(node->left, key);
    else if (key > node->key) node->right = insert(node->right, key);
    return node;
}

int bstToVine(Node* grand) {
    int count = 0;
    Node* tmp = grand->right;
    while (tmp) {
        if (tmp->left) {
            Node* oldTmp = tmp;
            tmp = tmp->left;
            oldTmp->left = tmp->right;
            tmp->right = oldTmp;
            grand->right = tmp;
        } else {
            count++;
            grand = tmp;
            tmp = tmp->right;
        }
    }
    return count;
}

void printVine(Node* vine) {
    Node* current = vine->right;
    while (current) {
        printf("%d ", current->key);
        current = current->right;
    }
    printf("\n");
}

int main() {
    Node* root = newNode(0);
    int keys[] = {4, 1, 5, 6, 3, 8};
    for (int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        insert(root, keys[i]);
    }

    printf("Before vine:\n");
    // Для демонстрации начального дерева
    printf("0 4 1 5 6 3 8\n\n");

    int count = bstToVine(root);

    printf("vine in list:\n");
    printVine(root);

    printf("\nNumber of nodes: %d\n", count);

    return 0;
}
