#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;


/*
* poj 2187 with graham-scan
*/
struct point {
	int x;
	int y;
};

struct point p[50010];
int stack[50010]; // 凸包上的点
int check[50010]; // 判断点有没有被使用过 
int n;
int m; // 凸包上点的个数 

void init() {
	scanf("%d",&n);
	for(int i = 0; i < n; i++) {
		scanf("%d %d",&p[i].x, &p[i].y);
	}
	memset(check, 0 , sizeof(check));
	m = 0;
}

int cross(int x1, int y1, int x2, int y2) {
	return x1*y2 - x2*y1;
}

bool cmp(struct point p1, struct point p2) {
	return (p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x));
}

int isleft(int a, int b, int c) {
	int x1 = p[b].x - p[a].x;
	int y1 = p[b].y - p[a].y;
	int x2 = p[c].x - p[a].x;
	int y2 = p[c].y - p[a].y;
	int crossValue = cross(x1, y1, x2, y2);
	if (crossValue >= 0) {
		return 1;
	} else {
		return 0;
	}
}

int dis2(int a, int b) {
	int x = p[stack[a]].x - p[stack[b]].x;
	int y = p[stack[a]].y - p[stack[b]].y;
	return x*x + y*y;
}

void solve() {
	sort(p, p + n, cmp);
	m++;
	stack[m] = 0; // 第一个进入凸包的点 
	check[0] = 1;
	m++;
	stack[m] = 1;
	check[1] = 1;
	
	// 计算右链 
	for(int k = 2; k < n; k++) {
		while(!isleft(stack[m-1], stack[m], k)) {
			check[stack[m]] = 0;
			m--;
		}
		m++;
		stack[m] = k;
		check[k] = 1;
	}  
	
	// 计算左链
	for(int k = n - 1; k >= 0; k-- ) {
		if (check[k]) continue;
		while(!isleft(stack[m-1], stack[m], k)) {
			check[stack[m]] = 0;
			m--;
		}
		m++;
		stack[m] = k;
		check[k] = 1;
	} 

	// 现在stack里面的点就是凸包上面的点 
	
	int maxDis = 0;
	for(int i = 1; i < m; i++) 
		for(int j = i + 1; j <= m; j++) {
			int ans = dis2(i,j);
			if (ans > maxDis) {
				maxDis = ans; 
			}
		}
		printf("%d\n", maxDis);
}

int main() {
	init();
	solve();
	return 0;
}