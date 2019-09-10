/* Joseph Soukup
	Steven Mulvey */

typedef struct Node {
    tuple        data;
    char         color;
	int			 free;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

typedef struct tree {
	Node *root;
} tree;

Node* newNode(tuple data);

void leftRotate(tree* maple, Node *x);

void rightRotate(tree* maple, Node *x);

void moveDown(Node* x, Node *nParent);

void swapColors(Node *x1, Node *x2);

void swapValues(Node *u, Node *v);

int isOnLeft(Node* pt);

void fixRedRed(tree* maple, Node *x);

Node *successor(Node *x);

Node *BSTreplace(Node *x);

Node *sibling(Node* pt);

void deleteNode(tree* maple, Node *v);

int hasRedChild(Node* pt);

void fixDoubleBlack(tree* maple, Node *x);

void deleteByVal(tree* maple, tuple n);

Node* BSTInsert(Node *root, Node *pt);

void rotateLeft(tree* maple, Node *pt);

void rotateRight(tree* maple, Node *pt);

void fixViolation(tree* maple, Node *pt);

void insert(tree* maple, tuple data);

Node *search(tree* maple, tuple n);

int main();
