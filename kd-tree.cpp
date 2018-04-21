#include<bits/stdc++.h>
using namespace std;

const int k = 2;
#define COUNT 10
vector<vector<double> > v;
// A structure to represent node of kd tree
struct Node
{
    double point[k]; // To store k dimensional point
    Node *left, *right;
};


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

        pivot = md->point[curr_dim];
        swap(md, end - 1);
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

bool isInrange(double arr[], vector<double> range_x, vector<double> range_y) {
    if(arr[0] >= range_x[0] && arr[0] <= range_x[1] && arr[1] >= range_y[0] && arr[1] <= range_y[1])
        return true;
    return false;
}
bool isLeaf(Node* root) {
    if(root->left == NULL && root->right == NULL)
        return true;
    return false;
}
int isIntersect(double arr[], vector<double> range_x, vector<double> range_y, int dim) {
    if(dim%2) {
        if(arr[1] >= range_y[0] && arr[1] <= range_y[1]) {
            return 1;
        }
        else if(arr[1] >= range_y[1]) {
            return 2;
        }
        else if(arr[1] <= range_y[0]) {
            return 3;
        }
    }
    else {
        if(arr[0] >= range_x[0] && arr[0] <= range_x[1]) {
            return 1;
        }
        else if(arr[0] >= range_x[1]) {
            return 2;
        }
        else if(arr[0] <= range_x[0]) {
            return 3;
        }
    }
}

void pointsInRectangle(Node *root, vector<double> range_x, vector<double> range_y, int dim) {
    if(isLeaf(root)) {
        if(isInrange(root->point, range_x, range_y)) {
            vector<double> temp;
            temp.push_back(root->point[0]);
            temp.push_back(root->point[1]);
            v.push_back(temp);
        }
    }
    else if(isIntersect(root->point, range_x, range_y, dim) == 1) {
        pointsInRectangle(root->left, range_x, range_y, (dim+1)%k);
        pointsInRectangle(root->right, range_x, range_y, (dim+1)%k);
    }
    else if(isIntersect(root->point, range_x, range_y, dim) == 2) {
        pointsInRectangle(root->left, range_x, range_y, (dim+1)%k);
    }
    else if(isIntersect(root->point, range_x, range_y, dim) == 3) {
        pointsInRectangle(root->right, range_x, range_y, (dim+1)%k);
    }

}

// Driver program to test above functions
int main() {
    Node *root = NULL;
    int points[][k] = {{3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19}};
    int n = sizeof(points)/sizeof(points[0]);
    Node nodes[n];
    for(int i = 0; i < n; i++) {
        nodes[i] = (*newNode(points[i]));
    }
    root = buildKDtreeRec(nodes, n, 0, k);
    //print2D(root);

    vector<double> range_x(2);
    vector<double> range_y(2);

    range_x[0] = 9;
    range_x[1] = 9;

    range_y[0] = 1;
    range_y[1] = 1;
    pointsInRectangle(root, range_x, range_y, 0);
    for(int i = 0; i < v.size(); i++) {
        for(int j = 0; j < 2; j++) {
            cout << v[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
