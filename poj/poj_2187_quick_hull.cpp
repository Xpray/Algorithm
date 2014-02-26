#include <stdio.h>

/*
*
*  poj 2187 with quick_hull
*/

typedef struct {
	int x;
	int y;
}point;

point p[50005];
point stack[50005]; // 凸包点的下标 
int n, m;

int cmp(point a, point b) {
	if (a.y < b.y || (a.y == b.y && a.x < b.x)) {
		return 1;
	} else {
		return 0;
	}
}

int ABS(int x) {
	return x > 0 ? x : -x;
}

void swap(int a, int b) {
	point tmp = p[a];
	p[a] = p[b];
	p[b] = tmp;
}

int dis2(point a, point b) {
	int X = a.x - b.x;
	int Y = a.y - b.y;
	return X*X + Y*Y;
}

int cross(point a, point b, point c) {
	int x1 = b.x - a.x;
	int y1 = b.y - a.y;
	int x2 = c.x - a.x;
	int y2 = c.y - a.y;
	return x1*y2 - x2*y1;
}

int isLeft(point a, point b, point c) {
	int cr = cross(a, b, c);
	if (cr >= 0) {
		return 1;
	} else {
		return 0;
	}
}

void quickHull(int l, int r, point a, point b) {
	int y;
	if (a.x == b.x && a.y == b.y) {
		// a == b时, 找距离a最远的点
		int max = 0;
		for(int i = l; i <= r; i++) {
			int dis = dis2(a, p[i]);
			if (dis > max) {
				max = dis;
				y = i;
			}
		} 
	} else {
		// 否则找距离线段ab 最远的点
		// 用叉积的绝对值来比较
		int max = 0;
		for(int i = l; i <= r; i++) {
			int area = ABS(cross(p[i], a, b));
			if (area >= max) {  // 这个最远点可能与ab共线，这样面积为0, y指向随机值 等号不能少 
				max = area;
				y = i;
			}
		} 
	}
	
	// 通过y来做划分 
	// y-->b 左侧的点
	point Y = p[y];
	
	int right = r + 1;
	for(int i = r; i >= l; i--) {
		if (p[i].x == Y.x && p[i].y == Y.y) continue;
		if (isLeft(Y, b, p[i])) {
			right--;
			swap(right, i);
		} 
	} 
	
	int left = l - 1;
	for(int i = l; i <= right - 1; i++) {
		if (p[i].x == Y.x && p[i].y == Y.y) continue;
		if (isLeft(a, Y, p[i])) {
			left++;
			swap(left, i);
		}
	}
	
	if (right <= r) {
		quickHull(right, r, Y, b);
	}
	stack[++m] = Y;;
	if (left >= l) {
		quickHull(l, left, a, Y);
	}
	
}


void solve() {
	for(int i = 1; i < n; i++) {
		if (cmp(p[i], p[0])) {
			swap(i, 0);
		} 
	}
	m = 1;
	stack[1] = p[0]; // 左下的点 
	quickHull(1, n-1, p[0], p[0]);
	int ans = 0;
	for(int i = 1; i < m; i++)
		for(int j = i + 1; j <= m; j++) {
			int t = dis2(stack[i], stack[j]);
			if (t > ans) {
				ans = t;
			}
		}
	printf("%d\n", ans);
}

int main() {
	scanf("%d",&n);
	for(int i = 0; i < n; i++) {
		scanf("%d %d",&p[i].x, &p[i].y);
	}
	solve();
	return 0;
}
