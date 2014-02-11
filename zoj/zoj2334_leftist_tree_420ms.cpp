#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** zoj 2334 Monkey King with leftist tree
*/

typedef struct node {
	struct node *parent;
	struct node *left;
	struct node *right;
	int value;
	int dis;
}node;

int n,m;
int a[100010];
node *f[100010];

/*
** link x to y  
*/
node *union_heap(node *x, node *y, node *f) {
	if (y == NULL) {
		x->parent = f;
		return x;
	} else {
		node *temp = NULL;
		if (x->value <= y->value) {
			temp = y;
			y->right = union_heap(x, y->right, y);
		} else {
			temp = x;
			x->right = union_heap(y, x->right, x);
		}
		int l = temp->left == NULL ? 0 : temp->left->dis;
		int r = temp->right == NULL ? 0 : temp->right->dis;
		temp->dis = l < r ? l + 1 : r + 1;
		if (l < r) {
			node *temp = y->left;
			y->left = y->right;
			y->right = temp;
		}
		temp->parent = f;
		return temp;
	}
}

node *merge_heap(node *x, node *y) {
	if (x == NULL) return y;
	if (y == NULL) return x;
	if (x->value <= y->value) 
		return union_heap(x, y, NULL);
	else
		return union_heap(y, x, NULL);
}

node *extract(node **x) {
	node *ans = *x;
	node *l = (*x)->left;
	node *r = (*x)->right;
	ans->left = NULL;
	ans->right = NULL;
	ans->dis = 1;
	if (l != NULL) l->parent = NULL;
	if (r != NULL) r->parent = NULL;
	*x = merge_heap(l, r);
	return ans;
}

int get_top(node *x) {
	return x->value;
}

void init() {
	for(int i = 1; i <= 100000; i++) {
		f[i] = (node *)malloc(sizeof(node));
	}
}

void Read() {
	//scanf("%d",&n);
	for(int i = 1; i <= n; i++) {
		scanf("%d", &(f[i]->value));
		f[i]->parent = NULL;
		f[i]->left = NULL;
		f[i]->right = NULL;
		f[i]->dis = 1;
	}
}

node *find(node *x) {
	while(x->parent != NULL) {
		x = x->parent;
	}
	return x;
}

void solve() {
	scanf("%d",&m);
	int a,b;
	while(m--) {
		scanf("%d %d",&a,&b);
		node *x = find(f[a]);
		node *y = find(f[b]);
		if (x == y) {
			printf("-1\n");
		} else {
			node *t1 = extract(&x);
			node *t2 = extract(&y);
			t1->value /= 2;
			t2->value /= 2;
			x = merge_heap(x, y);
			x = merge_heap(x, t1);
			x = merge_heap(x, t2);
			int ans = get_top(x);
			printf("%d\n", ans);
		}
	}
}

void clean() {
	for(int i = 1; i <= 100000; i++) free(f[i]);	
}

int main() {
	init();
	while(scanf("%d",&n)!=EOF) {
		Read();
		solve();
	}
	clean();
	return 0;
}
