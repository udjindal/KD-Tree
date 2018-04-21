#include<bits/stdc++.h>
using namespace std;

const int k = 2;
int n;
#define COUNT 10

// A structure to represent node of kd tree
struct Node
{
    double point[k]; // To store k dimensional point
    Node *left, *right;
};


bool arePointsSame(int point1[], int point2[]) {
    // Compare individual pointinate values
    for (int i = 0; i < k; ++i)
        if (point1[i] != point2[i])
            return false;

    return true;
}

void print2DUtil(Node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%f %f\n", root->point[0], root->point[1]);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(Node *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
}

Node* newNode(int arr[]) {
    Node* temp = new Node;
    for (int i=0; i < k; i++)
       temp->point[i] = arr[i];

    temp->left = temp->right = NULL;
    return temp;
}

void swap(Node *a, Node *b) {
    double temp[k];
    memcpy(temp, a->point, sizeof(temp));
    memcpy(a->point, b->point, sizeof(temp));
    memcpy(b->point, temp, sizeof(temp));
}

Node* findMedian(Node *start, Node *end, int curr_dim) {
    if(end <= start)
        return NULL;
    if(end == start + 1)
        return start;
    Node *p, *store, *md;
    if((end - start)%2) {
        md = start + (end - start)/2;
    }
    else {
        md = start + (end - start)/2 - 1;
    }
    double pivot;
    while(true) {

        pivot = (end-1)->point[curr_dim];
        for (store = p = start; p < end; p++) {
            if (p->point[curr_dim] < pivot) {
                swap(p, store);
                store++;
            }
        }
        swap(store, end - 1);
        if (store->point[curr_dim] == md->point[curr_dim])
            return md;

        if (store > md)
            end = store;
        else
            start = store;
    }
}

Node* buildKDtreeRec(Node *t, int len, int i, int dim) {
    Node *med = new Node;
    Node *med2;
    if(len == 1) {
        return t;
    }
    med2 = findMedian(t, t + len, i);
    med->point[0] = med2->point[0];
    med->point[1] = med2->point[1];
    i = (i+1)%dim;
    med->left = buildKDtreeRec(t, med2 + 1 - t, i, dim);
    med->right = buildKDtreeRec(med2 + 1, (t + len) - (med2 + 1), i, dim);
    return med;
}

// Driver program to test above functions
int main() {
    Node *root = NULL;
    int points[][k] = {{3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19}};
    n = sizeof(points)/sizeof(points[0]);
    Node nodes[n];
    for(int i = 0; i < n; i++) {
        nodes[i] = (*newNode(points[i]));
    }
    root = buildKDtreeRec(nodes, n, 0, k);
    print2D(root);

    return 0;
}
