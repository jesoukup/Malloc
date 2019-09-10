/* Joseph Soukup
	Steven Mulvey */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "537malloc.h"
#include "537tree.h"
#include <stdint.h>

tree *maple = NULL;

tree * constructTree() {
	tree * maple = malloc(sizeof(tree));
	maple->root = NULL;
	return maple;
}

void *malloc537(size_t size) {
	if (maple == NULL){
		maple = constructTree();
	}
	int * temp;
		tuple * t = malloc(sizeof(tuple));
			t->len = size; 
			temp = (int*)malloc(size);
		t->addr = (uintptr_t)&(*temp);
	if (size == 0) {
		printf("Warning: size is 0!");
	}
	tuple e = *t;
	insert(maple, e); 
	return temp;
}

void free537(void *ptr) {
	// Check to make sure malloc537 was used to malloc
    Node * needsFree = search537(ptr);
    if (needsFree == NULL) {
      printf("Error: Memory has not been previously allocated at this address");
      exit(-1);
    }
    if (needsFree->free == 1) {
    printf("Error: Attempting to double free");
    exit(-1);
    }
    else if (needsFree->free == 0){
    needsFree->free = 1;
    free(ptr);
    }
	}



struct Node *search537(uintptr_t *addr) {
  Node *temp = maple->root;
  while (temp != NULL) {
    if (temp->data.addr > (uintptr_t)&(*addr)) {
      //printf("Is smaller than");
      if (temp->left == NULL) {
        printf("Warning: Memory address 2 not allocated");
        break;
        }
       else { 
      temp = temp->left;
      }
    }
    else if (temp->data.addr == (uintptr_t)&(*addr)) {
    //printf("Is equal to");
    break;
    }
    else {
    //printf("Is larger than");
      if (temp->right == NULL) {
        uintptr_t add = temp->data.addr + temp->data.len;
        if (add > (uintptr_t)&(*addr)){
        printf("The memory is allocated but not a node");
        }
        else {
        printf("Warning: Memory address too big");
        break;
        }
        }
      else {
    temp = temp->right;
    }
    }
}
return temp;
}


void *realloc537(void *ptr, size_t size) {
	Node *temp = maple->root;
	if (ptr == NULL) {
		return malloc537(size);
	}
	else if (size == 0) {
		if (ptr != NULL) {
			free537(ptr);
			return ptr;
		}
	} 
	else {
		Node *node = search537(ptr);
		deleteByVal(maple, node->data);
		int * temp;
		tuple * t = malloc(sizeof(tuple));
		t->len = size; 
		temp = (int*)malloc(size);
		t->addr = (uintptr_t)&(*temp);
		if (size == 0) {
			printf("Warning: size is 0!");
		}
		tuple e = *t;
		insert(maple, e); 
	}
	return temp;
}

void memcheck537(void *ptr, size_t size) {
	Node *temp = maple->root; 
	uintptr_t ref = (uintptr_t)ptr + size;
	while (temp != NULL) { 
		if (((uintptr_t)ptr + size < temp->data.addr + temp->data.len) || ((uintptr_t)ptr < temp->data.addr)) { 
			if (temp->left == NULL) {
				printf("Checked memory is partially or fully unallocated.\n");
				exit(-1); 
			}
			else
				temp = temp->left; 
		} else if ((ref == temp->data.addr + temp->data.len) || ((uintptr_t)ptr == temp->data.addr))  { 
		if (temp->free == 0) {
			printf("Checked memory is allocated.\n");
			break; 
		}
		} else if (((uintptr_t)ptr + size > temp->data.addr + temp->data.len) || ((uintptr_t)ptr > temp->data.addr)) {
			if (temp->right == NULL) {
				printf("Checked memory is partially or fully unallocated.\n");
				exit(-1);
			}
			else
				temp = temp->right; 
		}
	} 
}
/*
int main () {
	uintptr_t *test = malloc537(6);
	memcheck537(test, sizeof(test));
	int *five = malloc(5);
	memcheck537(five, sizeof(five));
}*/
