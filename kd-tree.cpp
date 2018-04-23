#include <bits/stdc++.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

const int k = 2;
int points[][k] = {{3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19}};

vector<double> range_x(2);
vector<double> range_y(2);
vector<pair <int, bool> > median_y;

#define COUNT 10
vector<vector<double> > v;
// A structure to represent node of kd tree
struct Node {
    double point[k]; // To store k dimensional point
    Node *left, *right;
};

void reportSubtree(Node *root) {
    // if node is null, return
    if (!root)
        return;

    // if node is leaf node, print its data
    if (!root->left && !root->right)
    {
        vector<double> temp;
        temp.push_back(root->point[0]);
        temp.push_back(root->point[1]);
        v.push_back(temp);
        return;
    }

    // if left child exists, check for leaf
    // recursively
    if (root->left)
       reportSubtree(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
       reportSubtree(root->right);
}

void print2DUtil(Node *root, int space) {
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
void print2D(Node *root) {
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
    if(i%2) {
        median_y.push_back(make_pair(med2->point[1], true));
    }
    else {
        median_y.push_back(make_pair(med2->point[0], false));
    }
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

int rectPos(Node *root, vector<double> range_x_rec, vector<double> range_y_rec, vector<double> range_x_reg, vector<double> range_y_reg, int dim) {
    if(range_x_reg[0] > range_x_rec[0] && range_x_reg[1] <= range_x_rec[1] && range_y_reg[0] > range_y_rec[0] && range_y_reg[1] <= range_y_rec[1])
        return 1;
    else if (range_x_rec[0] > range_x_reg[1] || range_x_rec[1] < range_x_reg[0])
        return 2;
    else {
        if(dim%2) {
            if(range_y_rec[1] <= root->point[1])
                return 3;
            else if(range_y_rec[0] > root->point[1])
                return 4;
            else
                return 5;
        }
        else {
            if(range_x_rec[1] <= root->point[0])
                return 3;
            else if(range_x_rec[0] > root->point[0])
                return 4;
            else
                return 5;
        }
    }
}

void pointsInRectangle(Node *root, vector<double> range_x_rec, vector<double> range_y_rec, vector<double> range_x_reg, vector<double> range_y_reg, int dim) {
    if(isLeaf(root)) {
        if(isInrange(root->point, range_x_rec, range_y_rec)) {
            vector<double> temp;
            temp.push_back(root->point[0]);
            temp.push_back(root->point[1]);
            v.push_back(temp);
        }
    }
    else {
        cout << root->point[0] << " " << root->point[1] << " " <<rectPos(root, range_x_rec, range_y_rec, range_x_reg, range_y_reg, dim) << '\n';
        vector<double> range_y_reg_left(2);
        vector<double> range_y_reg_right(2);
        vector<double> range_x_reg_left(2);
        vector<double> range_x_reg_right(2);
        if(dim%2) {
            range_y_reg_left[0] = range_y_reg[0];
            range_y_reg_left[1] = root->point[1];
            range_y_reg_right[0] = root->point[1];
            range_y_reg_right[1] = range_y_reg[1];
        }
        else {
            range_x_reg_left[0] = range_x_reg[0];
            range_x_reg_left[1] = root->point[0];
            range_x_reg_right[0] = root->point[0];
            range_x_reg_right[1] = range_x_reg[1];
        }
        // region lies inside the rectange
        if(rectPos(root, range_x_rec, range_y_rec, range_x_reg, range_y_reg, dim) == 1) {
            reportSubtree(root);
        }
        // no intersection between rectangle and region
        else if(rectPos(root, range_x_rec, range_y_rec, range_x_reg, range_y_reg, dim) == 2) {
            return;
        }
        // will lie left side after new partition
        else if(rectPos(root, range_x_rec, range_y_rec, range_x_reg, range_y_reg, dim) == 3) {
            if(dim%2) {
                pointsInRectangle(root->left, range_x_rec, range_y_rec, range_x_reg, range_y_reg_left, (dim+1)%2);
            }
            else {
                pointsInRectangle(root->left, range_x_rec, range_y_rec, range_x_reg_left, range_y_reg, (dim+1)%2);
            }
        }
        // will lie right side after new partition
        else if(rectPos(root, range_x_rec, range_y_rec, range_x_reg, range_y_reg, dim) == 4) {
            if(dim%2) {
                pointsInRectangle(root->right, range_x_rec, range_y_rec, range_x_reg, range_y_reg_right, (dim+1)%2);
            }
            else {
                pointsInRectangle(root->right, range_x_rec, range_y_rec, range_x_reg_right, range_y_reg, (dim+1)%2);
            }
        }
        // will intersect the new partition
        else if(rectPos(root, range_x_rec, range_y_rec, range_x_reg, range_y_reg, dim) == 5) {
            if(dim%2) {
                pointsInRectangle(root->left, range_x_rec, range_y_rec, range_x_reg, range_y_reg_left, (dim+1)%2);
                pointsInRectangle(root->right, range_x_rec, range_y_rec, range_x_reg, range_y_reg_right, (dim+1)%2);
            }
            else {
                pointsInRectangle(root->left, range_x_rec, range_y_rec, range_x_reg_left, range_y_reg, (dim+1)%2);
                pointsInRectangle(root->right, range_x_rec, range_y_rec, range_x_reg_right, range_y_reg, (dim+1)%2);
            }
        }
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        string s;
        void * font = GLUT_BITMAP_8_BY_13;;
        for (int i = 0; i < median_y.size(); i++) {
            glBegin(GL_LINE_STRIP);
                if (median_y[i].second) {
                    glColor3f(1.0, 1.0, 0.0);

                    glVertex3f(-100.0, median_y[i].first, 0.0);
                    glVertex3f(100.0, median_y[i].first, 0.0);
                } else {
                    glColor3f(1.0, 0.4, 0.5);

                    glVertex3f(median_y[i].first, -100.0, 0.0);
                    glVertex3f(median_y[i].first, 100, 0.0);
                }
            glEnd();

            if (!median_y[i].second) {
                glColor3f(0.4f, 0.2f, 0.5f);
                glRasterPos2f(median_y[i].first + 0.2f, -4.0f);
                s = to_string(i+1);

                for (string::iterator i = s.begin(); i != s.end(); ++i) {
                    char c = *i;
                    glutBitmapCharacter(font, c);
                }
            } else {
                glColor3f(0.2f, 0.5f, 0.5f);
                glRasterPos2f(-4.0f, median_y[i].first + 0.2f);
                s = to_string(i+1);

                for (string::iterator i = s.begin(); i != s.end(); ++i) {
                    char c = *i;
                    glutBitmapCharacter(font, c);
                }
            }
        }
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(8);
    glBegin(GL_POINTS);
        for (int i = 0; i < sizeof(points)/sizeof(points[0]); i++) {
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(points[i][0], points[i][1]);
        }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f (0.5, 0.6, 0.3);
    glBegin(GL_LINE_STRIP);
        glVertex2f(range_x[0], range_y[0]);
        glVertex2f(range_x[1], range_y[0]);
        glVertex2f(range_x[1], range_y[1]);
        glVertex2f(range_x[0], range_y[1]);
        glVertex2f(range_x[0], range_y[0]);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-25.0, 25.0, -25.0*(GLfloat)h/(GLfloat)w,
                25.0*(GLfloat)h/(GLfloat)w, -25.0, 25.0);
    else
        glOrtho(-50.0*(GLfloat)w/(GLfloat)h,
                50.0*(GLfloat)w/(GLfloat)h, -50.0, 50.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Driver program to test above functions
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize (750, 750);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("KD-Tree");

    Node *root = NULL;
    int n = sizeof(points)/sizeof(points[0]);
    Node nodes[n];
    for(int i = 0; i < n; i++) {
        nodes[i] = (*newNode(points[i]));
    }
    root = buildKDtreeRec(nodes, n, 0, k);
    print2D(root);

    vector<double> range_x_reg(2);
    vector<double> range_y_reg(2);

    range_x[0] = 2;
    range_x[1] = 17;

    range_y[0] = 1;
    range_y[1] = 19;

    range_x_reg[0] = -1000;
    range_x_reg[1] = 1000;

    range_y_reg[0] = -1000;
    range_y_reg[1] = 1000;

    pointsInRectangle(root, range_x, range_y, range_x_reg, range_y_reg, 0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    for(int i = 0; i < v.size(); i++) {
        for(int j = 0; j < 2; j++) {
            cout << v[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
