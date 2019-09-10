/* Joseph Soukup
	Steven Mulvey */

#include<stdio.h>
#include<stdlib.h>
#include "537malloc.h"
#include "537tree.h"

Node* newNode(tuple data) {
	Node *temp   = (Node*) malloc(sizeof(Node));
    temp->data   = data;
    temp->color  = 'R';
    temp->left   = NULL;
    temp->right  = NULL;
    temp->parent = NULL;

    return temp;
}

void leftRotate(tree* maple, Node *x) { 
	Node *nParent = x->right; 
	if (x == maple->root) 
	maple->root = nParent; 
 
	if (x->parent != NULL) { 
		if (isOnLeft(x) == 1) { 
			x->parent->left = nParent; 
		} else { 
			x->parent->right = nParent; 
		} 
    } 
    nParent->parent = x->parent; 
    x->parent = nParent;
	x->right = nParent->left; 
	if (nParent->left != NULL) 
	nParent->left->parent = x; 
	nParent->left = x; 
} 

void rightRotate(tree* maple, Node *x) { 
	Node *nParent = x->left; 
	if (x == maple->root) 
	maple->root = nParent; 
	if (x->parent != NULL) { 
		if (isOnLeft(x) == 1) { 
			x->parent->left = nParent; 
		} else { 
			x->parent->right = nParent; 
		} 
    } 
    nParent->parent = x->parent; 
    x->parent = nParent;
	x->left = nParent->right; 
	if (nParent->right != NULL) 
	nParent->right->parent = x; 
	nParent->right = x; 
} 

void moveDown(Node* x, Node *nParent) { 
	if (x->parent != NULL) { 
		if (isOnLeft(x) == 1) { 
			x->parent->left = nParent; 
		} else { 
			x->parent->right = nParent; 
		} 
    } 
    nParent->parent = x->parent; 
    x->parent = nParent; 
} 

void swapColors(Node *x1, Node *x2) { 
	Node* temp = NULL; 
	temp->color = x1->color; 
	x1->color = x2->color; 
	x2->color = temp->color; 
} 

void swapValues(Node *u, Node *v) { 
	tuple temp; 
	temp = u->data; 
	u->data = v->data; 
	v->data = temp; 
} 

int isOnLeft(Node* pt) { 
	if (pt == pt->parent->left) {
		return 1;
	}
	return 0; 
}

void fixRedRed(tree* maple, Node *x) { 
	if (x == maple->root) { 
	x->color = 'B'; 
	return; 
	} 
	Node *parent = x->parent, *grandparent = parent->parent;
	Node *uncle;
	if (x->parent->left == x) 
		uncle = x->parent->parent->right;
    else 
		uncle = x->parent->parent->left; 

	if (parent->color != 'B') {
	if (uncle != NULL) {
	if (uncle->color == 'R') { 
		parent->color = 'B'; 
		uncle->color = 'B'; 
		grandparent->color = 'R'; 
		fixRedRed(maple, grandparent);
	}
	} else { 
		if (isOnLeft(parent) == 1) { 
		if (isOnLeft(x) == 1) { 
			swapColors(parent, grandparent); 
		} else { 
			leftRotate(maple, parent);
			swapColors(x, grandparent); 
		}  
		rightRotate(maple, grandparent); 
		} else { 
		if (isOnLeft(x) == 1) { 
			rightRotate(maple, parent);
			swapColors(x, grandparent); 
		} else { 
			swapColors(parent, grandparent); 
		} 
		leftRotate(maple, grandparent);
		} 
	} 
	} 
} 

Node *successor(Node *x) { 
	Node *temp = x; 

	while (temp->left != NULL) 
	temp = temp->left; 

	return temp; 
} 

Node *BSTreplace(Node *x) { 
	if ((x->left != NULL) && (x->right != NULL)) 
	return successor(x->right); 
	if ((x->left == NULL) && (x->right == NULL)) 
	return NULL; 
	if (x->left != NULL) 
	return x->left; 
	else
	return x->right; 
} 

Node *sib(Node* pt) { 
    if (pt->parent == NULL) 
      return NULL; 
    if (isOnLeft(pt) == 1) 
		return pt->parent->right; 
    return pt->parent->left; 
  } 

void deleteNode(tree* maple, Node *v) { 
	Node *u = BSTreplace(v); 
	if (u == v) {
		u = NULL;
	}
	int uvBlack = 0;
	if (u != NULL) {
	if ((u->color == 'B') && (v->color == 'B')) { 
		uvBlack = 1;
	}
	}
	Node *parent = v->parent; 

	if (u == NULL) { 
	if (v == maple->root) {  
		maple->root = NULL; 
	} else { 
		if (uvBlack == 1) { 
		fixDoubleBlack(maple, v); 
		} else { 
		if (sib(v) != NULL)  
			sib(v)->color = 'R'; 
		} 
		if (isOnLeft(v) == 1) { 
		parent->left = NULL; 
		} else { 
		parent->right = NULL; 
		} 
	} 
	free(v);
	v = NULL;
	return; 
	} 

	if ((v->left == NULL) || (v->right == NULL)) { 
	if (v == maple->root) { 
		v->data = u->data; 
		v->left = v->right = NULL; 
		free(u); 
		u = NULL;
	} else { 
		if (isOnLeft(v) == 1) { 
		parent->left = u; 
		} else { 
		parent->right = u; 
		} 
		free(v); 
		v = NULL;
		u->parent = parent; 
		if (uvBlack) { 
		fixDoubleBlack(maple, u); 
		} else { 
		u->color = 'B'; 
		} 
	} 
	return; 
	} 
	swapValues(u, v); 
	deleteNode(maple, u); 
} 

int hasRedChild(Node* pt) { 
	if ((pt->left != NULL) || (pt->right != NULL)) {
    if ((pt->left->color == 'R') || (pt->right->color == 'R')) { 
		return 1;
	}
	}
	return 0;
} 

void fixDoubleBlack(tree* maple, Node *x) { 
	if (x == maple->root) 
	return; 
	Node *sibling;
	sibling = sib(x);
	Node *parent = x->parent; 
	if (sibling == NULL) {  
	fixDoubleBlack(maple, parent); 
	} else { 
	if (sibling->color == 'R') { 
		parent->color = 'R'; 
		sibling->color = 'B'; 
		if (isOnLeft(sibling) == 1) { 
		rightRotate(maple, parent); 
		} else { 
		leftRotate(maple, parent); 
		} 
		fixDoubleBlack(maple, x); 
	} else { 
		if (hasRedChild(sibling) == 1) { 
		if (sibling->left != NULL) {
		if (sibling->left->color == 'R') { 
			if (isOnLeft(sibling) == 1) { 
			sibling->left->color = sibling->color; 
			sibling->color = parent->color; 
			rightRotate(maple, parent); 
			} else { 
			sibling->left->color = parent->color; 
			rightRotate(maple, sibling); 
			leftRotate(maple, parent); 
			} 
		}
		} else { 
			if (isOnLeft(sibling) == 1) { 
			sibling->right->color = parent->color; 
			leftRotate(maple, sibling); 
			rightRotate(maple, parent);
			} else {  
			sibling->right->color = sibling->color; 
			sibling->color = parent->color; 
			leftRotate(maple, parent);
			} 
		} 
		parent->color = 'B'; 
		} else { 
		sibling->color = 'R'; 
		if (parent->color == 'B') 
			fixDoubleBlack(maple, parent); 
		else
			parent->color = 'B'; 
		} 
	} 
	} 
} 

void deleteByVal(tree* maple, tuple n) { 
	if (maple->root == NULL) 
	return; 
	Node *v = search(maple, n);
	if (v == NULL) { 
	printf("No node found to delete with value."); 
	return; 
	} 
	deleteNode(maple, v); 
} 

Node* BSTInsert(Node *root, Node *pt) 
{ 
    if (root == NULL) 
		return pt; 
    if (pt->data.addr < root->data.addr) 
    { 
        root->left = BSTInsert(root->left, pt); 
        root->left->parent = root; 
    } 
    else if (pt->data.addr > root->data.addr) 
    { 
        root->right = BSTInsert(root->right, pt); 
        root->right->parent = root; 
    } 
    return root; 
} 

void rotateLeft(tree* maple, Node *pt) 
{ 
    Node *pt_right = pt->right; 
    pt->right = pt_right->left; 
    if (pt->right != NULL) 
        pt->right->parent = pt; 
    pt_right->parent = pt->parent; 
    if (pt->parent == NULL) 
        maple->root = pt_right; 
    else if (pt->data.addr == pt->parent->left->data.addr) 
        pt->parent->left = pt_right; 
    else
        pt->parent->right = pt_right; 
    pt_right->left = pt; 
    pt->parent = pt_right; 
} 

void rotateRight(tree* maple, Node *pt) 
{ 
    Node *pt_left = pt->left; 
    pt->left = pt_left->right; 
    if (pt->left != NULL) 
        pt->left->parent = pt; 
    pt_left->parent = pt->parent; 
    if (pt->parent == NULL) 
        maple->root = pt_left; 
    else if (pt->data.addr == pt->parent->left->data.addr) 
        pt->parent->left = pt_left; 
    else
        pt->parent->right = pt_left; 
    pt_left->right = pt; 
    pt->parent = pt_left; 
} 

void fixViolation(tree* maple, Node *pt) 
{ 
    Node *parent_pt = NULL; 
    Node *grand_parent_pt = NULL; 
    while ((pt != maple->root) && (pt->color != 'B') && 
           (pt->parent->color == 'R')) 
    { 
        parent_pt = pt->parent; 
        grand_parent_pt = pt->parent->parent; 
        if (parent_pt == grand_parent_pt->left) 
        { 
            Node *uncle_pt = grand_parent_pt->right; 
            if (uncle_pt != NULL && uncle_pt->color == 'R') 
            { 
                grand_parent_pt->color = 'R'; 
                parent_pt->color = 'B'; 
                uncle_pt->color = 'B'; 
                pt = grand_parent_pt; 
            } 
  
            else
            { 
                if (pt == parent_pt->right) 
                { 
                    rotateLeft(maple, parent_pt); 
                    pt = parent_pt; 
                    parent_pt = pt->parent; 
                } 
                rotateRight(maple, grand_parent_pt); 
				char temp = parent_pt->color;
				parent_pt->color = grand_parent_pt->color;
				grand_parent_pt->color = temp;
                pt = parent_pt; 
            } 
        } 
        else
        { 
            Node *uncle_pt = grand_parent_pt->left; 
            if ((uncle_pt != NULL) && (uncle_pt->color == 'R')) 
            { 
                grand_parent_pt->color = 'R'; 
                parent_pt->color = 'B'; 
                uncle_pt->color = 'B'; 
                pt = grand_parent_pt; 
            } 
            else
            { 
                if (pt == parent_pt->left) 
                { 
                    rotateRight(maple, parent_pt); 
                    pt = parent_pt; 
                    parent_pt = pt->parent; 
                } 
                rotateLeft(maple, grand_parent_pt); 
				char temp = parent_pt->color;
				parent_pt->color = grand_parent_pt->color;
				grand_parent_pt->color = temp;
                pt = parent_pt; 
            } 
        } 
    } 
  
    maple->root->color = 'B'; 
} 

void insert(tree* maple, tuple data) 
{ 
    Node *pt = newNode(data); 
    maple->root = BSTInsert(maple->root, pt); 
    fixViolation(maple, pt); 
} 

Node *search(tree* maple, tuple n) { 
	Node *temp = maple->root; 
	while (temp != NULL) { 
	if (n.addr < temp->data.addr) { 
		if (temp->left == NULL) 
		break; 
		else
		temp = temp->left; 
	} else if (n.addr == temp->data.addr) { 
		break; 
	} else { 
		if (temp->right == NULL) 
		break; 
		else
		temp = temp->right; 
	} 
	} 

	return temp; 
} 
/*
int main()
{
	tree *maple = malloc(sizeof(tree));
	maple->root = NULL;
	struct tuple t[30];
	int addrcount = 0;
	int lencount = 0;
    for (int i = 0; i < 20; i++) {
		t[i].addr = addrcount;
		t[i].len = lencount;
        insert(maple, t[i]);
		addrcount++;
		lencount++;
    }
	print2D(maple);
	deleteByVal(maple, t[0]);
	print2D(maple);
	deleteByVal(maple, t[1]);
	insert(maple, t[0]);
	print2D(maple);
}*/


