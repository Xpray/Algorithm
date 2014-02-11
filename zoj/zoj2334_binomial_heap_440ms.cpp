#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "binomial_heap.h"

/**
*
*	zoj 2334 Monkey King with binomial heap
*
*/

typedef struct node {
	struct node *parent;
    struct node *child;
    struct node *next;
    struct node *prev;
    int degree;
    int value;
} node;

node *create_binomial_heap(void) {
	return NULL;
}

void drop_binomial_heap(node *bp) {
	if (bp == NULL) return;
	drop_binomial_heap(bp->child);
	drop_binomial_heap(bp->next);
	free(bp);
}

void link_tree(node *x, node *y) { // link x to y as a subtree of leftmost	
	y->degree++;
	x->next = y->child;
	if (y->child != NULL) y->child->prev = x;
	y->child = x;
	x->parent = y;
}

node *merge_heap(node *x, node *y) {
	node *head = NULL;
	node *tail = NULL;
	node *t = NULL;
	
	while(x && y) {
		if (x->degree <= y->degree) {
			t = x;
			x = x->next;
		} else {
			t = y;
			y = y->next;
		}

		if (head == NULL) {
			head = t;
			tail = t;
		} else {
			tail->next = t;
			t->prev = tail;
			tail = t;
		}
	}
	
	if(x != NULL) {
		if (head == NULL) {
			head = x;
		} else {
			tail->next = x;
			x->prev = tail;
		}
	}
	
	if (y != NULL ) {
		if (head == NULL) {
			head = y;
		} else {
			tail->next = y;
			y->prev = tail;
		}
	}
	return head;
}

node *union_heap(node *x, node *y) {
	
	if (x == NULL && y == NULL) return NULL;
	
	node *head = NULL;
	node *prev_x = NULL;
	node *next_x = NULL;

	head = merge_heap(x, y);
	x = head;	
	next_x = x->next;

	while(next_x != NULL) {
		if ((next_x->degree != x->degree) || (next_x->next != NULL && x->degree == next_x->next->degree)) {
			prev_x = x;
			x = next_x;
		} else {
			if (x->value >= next_x->value) {
				x->next = next_x->next;
				if (next_x->next != NULL) next_x->next->prev = x;
				link_tree(next_x, x);
			} else {
				if (prev_x == NULL) { 
					head = next_x;
					next_x->prev = NULL; // do not forget
				} else {
					prev_x->next = next_x;
					next_x->prev = prev_x;
					x->prev = NULL; // do not forget
				}
				link_tree(x, next_x);
				x = next_x;
			}
		}
		next_x = x->next;
	}
	return head;
}

void insert(node **current, int v) {
	
	//node *c = *current;
	
	node *new_node = (node *)malloc(sizeof(node));
	new_node->parent = NULL;
	new_node->child = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->degree = 0;
	new_node->value = v;
	
	if (*current == NULL) {
		*current = new_node;
	} else {
		*current = union_heap(*current, new_node);
	}
}

node *extract(node **head) { // ** in C or *& in C++
	int ans = -1;

	node *x = *head;
	node *y = NULL;
	node *prev_y = NULL;
	
	/*
	while(x) {
		if (x->value > ans) {
			ans = x->value;
			prev_y = y;
			y = x;
		}
		x = x->next;
	}

	if (prev_y == NULL) {
		*head = (*head)->next;
		if (*head != NULL) (*head)->prev = NULL;
	}
	else { 
		prev_y->next = y->next;  // kick y out
		if (y->next != NULL) y->next->prev = prev_y;
	}
	*/
	
	while(x) {
		if (x->value > ans) {
			ans = x->value;
			y =x;
		}
		x = x->next;
	}
	
	if (y->prev == NULL) {
		*head = (*head)->next;
		if ((*head) != NULL) (*head)->prev = NULL;	
	} else {
		y->prev->next = y->next;
		if (y->next != NULL) y->next->prev = y->prev;
	}
	
	
	// before clean y's link
	node *son = y->child;
	
	y->next = NULL;  // do not forget to clean y's link
	y->prev = NULL; 
	y->child = NULL;
	y->parent = NULL;
	y->degree = 0;   // clean all y's link
	
	node *new_node = NULL;
	node *now = NULL;

	while(son) {
		now = son;
		son = son->next;
		now->next = new_node;
		if (new_node != NULL) {
			new_node->prev = now;
		}
		new_node = now;
		now->parent = NULL;
		now->prev = NULL;
	}
	
	//if (new_node != NULL) new_node->prev = NULL; // very important

	// 这里必须要用*head 
	// union head && y's subtree
 	*head = union_heap(*head, new_node);
	//free(y); 这道题目的结点数目不会变少，不需要free 
	return y;
}

node *get_top(node *current) {
	int ans = -1;
	node *t = NULL;
	while(current != NULL) {
		if (current->value > ans){ 
			ans = current->value;
			t = current;
		}
		current = current->next;
	}
	return t;
}

node *root[100010]; // root[i]==节点的地址 
int a[100010];
int n,m;

void init() {
	for(int i=1;i<=100010;i++) {
		root[i] = NULL;
		insert(&root[i], 0);
	}
}

void init2() {
	for(int i=1;i<=n;i++) {
		root[i]->prev = NULL;
		root[i]->next = NULL;
		root[i]->degree = 0;
		root[i]->child = NULL;
		root[i]->parent = NULL;
		root[i]->value = a[i];
	}
}

node *find(int x) {
	node *now = root[x];
	
	// go up first
	while(now->parent != NULL) {
		now = now->parent;
	}
	
	// then go left
	while(now->prev != NULL) {
		now = now->prev;
	}
	
	return now;
}

void solve(int x, int y) {

	node *fx = find(x);
	node *fy = find(y);
	if (fx == fy) printf("-1\n");
	else {
		node *max1 = extract(&fx);
		node *max2 = extract(&fy);
		max1->value /= 2;
		max2->value /= 2;
		node *new_heap = union_heap(fx, fy);
		new_heap = union_heap(new_heap, max1);
		new_heap = union_heap(new_heap, max2);
		node *ans = get_top(new_heap);
		printf("%d\n", ans->value);
	}
}

void clean() {
	for(int i=1;i<=n;i++) {
		free(root[i]);
	}
}

int main() {
	int x,y;
	//srand(time(NULL));
	init();
	while(scanf("%d",&n)!=EOF) {
		for(int i=1;i<=n;i++) {
			scanf("%d",&a[i]);
		}
		init2();
		scanf("%d",&m);
		while(m--) {
			scanf("%d %d",&x, &y);
			solve(x, y);
		}
	}
	clean();
	return 0;
}
