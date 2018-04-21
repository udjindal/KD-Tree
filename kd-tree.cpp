#include<bits/stdc++.h>
using namespace std;

const int k = 2;
int n;
// A structure to represent node of kd tree
struct Node
{
    double point[k]; // To store k dimensional point
    Node *left, *right;
};

// A method to create a node of K D tree
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
        return min(end, start);
    Node *p, *store, *md = start + (end - start) / 2;
    double pivot;
    while(true) {
        pivot = md->point[curr_dim];

        swap(md, end -1);
        for (store = p = start; p < end; p++) {
            if (p->point[curr_dim] < pivot) {
                if (p != store)
                    swap(p, store);
                store++;
            }
        }
        if (store->point[curr_dim] == md->point[curr_dim])
            return md;

        if (store > md)
            end = store;
        else
            start = store;
    }
}

Node* buildKDtreeRec(Node *t, int len, int i, int dim) {
    Node *med;
    if(!len)
        return 0;
    med = findMedian(t, t+len, i);
    i = (i+1)%dim;
    med->left = buildKDtreeRec(t, med - t, i, dim);
    med->right = buildKDtreeRec(med+1, (t+len) - (med+1), i, dim);
    return med;
}

bool arePointsSame(int point1[], int point2[]) {
    // Compare individual pointinate values
    for (int i = 0; i < k; ++i)
        if (point1[i] != point2[i])
            return false;

    return true;
}


// Driver program to test above functions
int main() {
    struct Node *root = NULL;
    int points[][k] = {{3, 6}, {17, 15}, {13, 15}, {6, 12},
                       {9, 1}, {2, 7}, {10, 19}};

    n = sizeof(points)/sizeof(points[0]);
    Node nodes[n];
    for(int i = 0; i < n; i++) {
        nodes[i] = (*newNode(points[i]));
    }
    root = buildKDtreeRec(nodes, n, 0, k);
    return 0;
}
