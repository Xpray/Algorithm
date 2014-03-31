#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RET 9973


/*
* poj 1442 black box with treap
*/

typedef struct node {
	struct node *left;
	struct node *right;
	int key;
	int priority;
	int leftNum;
	int rightNum;
}node;

node *treap_create() {
	srand(time(NULL));
	return NULL;
}

node *create_node(int val) {
	node *t = (node *)malloc(sizeof(node));
	t->left = NULL;
	t->right = NULL;
	t->key = val;
	t->leftNum = 0;
	t->rightNum = 0;
	t->priority = rand() % RET;
	return t;
}

void left_rotate(node **v) {
	node *a = (*v)->right;
	if (a != NULL) {
		(*v)->right = a->left;
		(*v)->rightNum = a->left == NULL ? 0 : a->left->leftNum + a->left->rightNum + 1;
		a->left = (*v);
		a->leftNum = (*v)->leftNum + (*v)->rightNum + 1;
		*v = a;
	}
}

void right_rotate(node **v) {
	node *a = (*v)->left;
	if (a != NULL) {
		(*v)->left = a->right;
		(*v)->leftNum = a->right == NULL ? 0 : a->right->leftNum + a->right->rightNum + 1;
		a->right = (*v);
		a->rightNum = (*v)->leftNum + (*v)->rightNum + 1;
		*v = a;
	}
}

void treap_insert(node **v, int *val) {
	if (*v == NULL) {
		*v = create_node(*val);
	} else if (*val < (*v)->key) {
		(*v)->leftNum++;
		treap_insert(&((*v)->left), val);
		if ((*v)->left->priority < (*v)->priority) {
			right_rotate(v);
		}
	} else {
		treap_insert(&((*v)->right), val);
		(*v)->rightNum++;
		if((*v)->right->priority < (*v)->priority) {
			left_rotate(v);
		}
	}
}

void treap_remove(node **v, int val) {
	if ((*v)->key == val) {
		if ((*v)->left == NULL || ((*v)->right == NULL)) {
			node *tmp = *v;
			if ((*v)->right == NULL) {
				*v = (*v)->left;
			} else {
				*v = (*v)->right;
			}
			free(tmp);
		} else {
			if ((*v)->left->priority < (*v)->right->priority) {
				right_rotate(v);
				treap_remove(&((*v)->right), val);
			} else {
				left_rotate(v);
				treap_remove(&((*v)->left), val);
			}
		}  
	} else if (val < (*v)->key) {
		(*v)->leftNum--;
		treap_remove(&((*v)->left), val);
	}  else {
		(*v)->rightNum--;
		treap_remove(&((*v)->right), val);
	}
}

node *find(node *v, int val) {
	if (v == NULL) {
		return NULL;
	} 
	if (v->key == val) {
		return v;
	} else if (val < v->key) {
		return find(v->left, val);
	} else {
		return find(v->right, val);
	}
}

node *findK(node *v, int k) {
	if (v == NULL) {
		return NULL;
	}
	if (v->leftNum + 1 == k) {
		return v;
	} else if(k < v->leftNum + 1) {
		return findK(v->left, k);
	} else {
		return findK(v->right, k - v->leftNum - 1);
	}
}


int m[30099];
int n[30099];
int x,y;


int main() {
	scanf("%d %d", &x, &y);
	for(int i = 1; i <= x; i++) {
		scanf("%d", &m[i]);
	}
	for(int i = 1; i <= y; i++) {
		scanf("%d", &n[i]);
	}
	node *h = treap_create();
	int j = 0;
	for(int i = 1; i <= y; i++) {
		while(j < n[i]) {
			j++;
			treap_insert(&h, &m[j]);
		}
		node *tmp = findK(h, i);
		printf("%d\n", tmp->key);
	}
	return 0;
}




