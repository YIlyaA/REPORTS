#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
    struct Node *parent;
    int height;
} Node;

Node *newNode(int item) {
    Node *temp = (Node *) malloc(sizeof(Node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node *minValueNode(Node *node) {
    struct Node *current = node;
    while (current->left != NULL)
        current = current->left;

    return current;
}

Node *insert(Node *node, int key) {
    if (node == NULL) return newNode(key);

    if (key < node->key) {
        node->left = insert(node->left, key);
        node->left->parent = node;
    } else if (key > node->key) {
        node->right = insert(node->right, key);
        node->right->parent = node;
    }

    return node;
}

int bstToVine(Node* grand) {
    int count = 0;
    struct Node* tmp = grand->right;

    // Traverse until tmp becomes NULL
    while (tmp) {
        // If left exists for node
        // pointed by tmp then
        // right rotate it.
        if (tmp->left) {
            Node* oldTmp = tmp;
            tmp = tmp->left;
            oldTmp->left = tmp->right;
            tmp->right = oldTmp;
            grand->right = tmp;
        }
            // If left doesn't exist
            // add 1 to count and
            // traverse further right to
            // flatten remaining BST.
        else {
            count++;
            grand = tmp;
            tmp = tmp->right;
        }
    }
    return count;
}

// Function to compress given tree
// with its root as grand->right.
void compress(Node* grand, int m) {
    struct Node* tmp = grand->right;

    // Traverse and left-rotate root m times
    // to compress given vine form of BST.
    for (int i = 0; i < m; i++) {
        struct Node* oldTmp = tmp;
        tmp = tmp->right;
        grand->right = tmp;
        oldTmp->right = tmp->left;
        tmp->left = oldTmp;
        grand = tmp;
        tmp = tmp->right;
    }
}

// Function to implement the algorithm
Node* balanceBST(Node* root) {
    // create dummy node with value 0
    struct Node* grand = newNode(0);

    // assign the right of dummy node as our input BST
    grand->right = root;

    // get the number of nodes in input BST and
    // simultaneously convert it into right linked list.
    int count = bstToVine(grand);

    // gets the height of tree in which all levels
    // are completely filled.
    int h = log2(count + 1);

    // get number of nodes until second last level
    int m = pow(2, h) - 1;

    // Left rotate for excess nodes at last level
    compress(grand, count - m);

    // Left rotation till m becomes 0
    // Step is done as mentioned in algo to
    // make BST balanced.
    for (m = m / 2; m > 0; m /= 2) {
        compress(grand, m);
    }

    // return the balanced tree
    return grand->right;
}

// Function to print preorder traversal
// of Binary tree.
void preorderTraversal(Node* root) {
    if (!root)
        return;
    printf("%d ", root->key);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

/*void printBST(struct Node *root, int level) {
    //printf("");

    if (root != NULL) {
        printf("\nLevel: %d\n", level + 1);
        printf("Key: %d\n", root->key);
        if (root->right != NULL) printf("Right: %d\n", root->right->key);
        if (root->left != NULL) printf("Left: %d\n", root->left->key);
        printAvl(root->right, level + 1);
        printAvl(root->left, level + 1);
    }
    //printf("");

} */

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

double what_time_is_it() {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec * 1e-9;
}

#endif


int main() {
    Node *root = NULL;
    int n, i, num;
    double create_time = 0;
    double balance_time = 0;
    double find_min_time = 0;
    double start;
    double end;

    scanf("%d", &n);
    for (i = 0; i < n; ++i) {
        scanf("%d", &num);
        start = what_time_is_it();
        root = insert(root, num);
        end = what_time_is_it();
        create_time += (end - start);
    }
    start = what_time_is_it();
    minValueNode(root);
    Node *minNode = minValueNode(root);  // output min value
    //printf("Min: %d\n", minNode->key);
    end = what_time_is_it();
    find_min_time += (end - start);

//    printBST(root, 0);      // uncomment if you wnat to see nodes and keys before balancing
    start = what_time_is_it();
    root = balanceBST(root);
    end = what_time_is_it();
    balance_time += (end - start);
//    printBST(root, 0);     // uncomment if you wnat to see nodes and keys after balancing

    printf("create_time: %.20f\n", create_time);
    printf("balance_time: %.20f\n", balance_time);
    printf("find_min_time: %.20f\n", find_min_time);

    //preorderTraversal(root);
    return 0;
}
