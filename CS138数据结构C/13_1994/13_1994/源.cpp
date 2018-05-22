#include<iostream>
#pragma warning (disable:4996)
using namespace std;
const int Row = 505, Col=505;
int worldMap[Row][Col] = {0};
struct root {
	int x;//在根结点或空结点时x=y
	int y;
	root(int i=0,int j=0):x(i),y(j) {}
	root &operator=(const root &another) {
		x = another.x;
		y = another.y;
		return *this;
	}
};
root parentMap[Row][Col];
root findParent(int i,int j) {
	if (i==0||j==0) {
		return 0;
	}
	if (parentMap[i][j].x<0) {
		return root(i, j);
	}
	return parentMap[i][j] = findParent(parentMap[i][j].x, parentMap[i][j].y);
}
void unionParent(int firsti,int firstj,int secondi,int secondj) {
	if (firsti==secondi&&firstj==secondj) {
		return;
	}
	if (parentMap[firsti][firstj].x>parentMap[secondi][secondj].x) {
		parentMap[secondi][secondj].x += parentMap[firsti][firstj].x;
		parentMap[secondi][secondj].y += parentMap[firsti][firstj].y;
		parentMap[firsti][firstj].x = secondi;
		parentMap[firsti][firstj].y = secondj;
	}
	else {
		parentMap[firsti][firstj].x += parentMap[secondi][secondj].x;
		parentMap[firsti][firstj].y += parentMap[secondi][secondj].y;
		parentMap[secondi][secondj].x = firsti;
		parentMap[secondi][secondj].y = firstj;
	}
}
int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int m, n;
	cin >> n >> m;
	for (int i = 0; i <= n + 1;++i) {
		worldMap[i][0] = -1;
	}
	for (int i = 0; i <= m + 1;++i) {
		worldMap[0][i] = -1;
	}
	for (int i = 1; i <= n;++i) {
		for (int j = 1; j <= m;++j) {
			cin >> worldMap[i][j];
		}
	}
	
	for (int i = 1; i <= n;++i) {
		for (int j = 1; j <= m;++j) {
			if (worldMap[i][j] == 0) {
				if (worldMap[i - 1][j] != 0 && worldMap[i][j - 1] != 0) {//左上均为海洋，先暂时置为一个国家
					parentMap[i][j].x = -1;
					parentMap[i][j].y = -1;
				}
				else if (worldMap[i - 1][j] == 0 && worldMap[i][j - 1] != 0) {//左是陆地，上为海洋，那么它属于左边的国家，这里用union函数也可以
					int x = findParent(i-1, j).x;
					int y = findParent(i-1, j).y;
					//unionParent(x, y, i, j);
					parentMap[i][j].x = x;
					parentMap[i][j].y = y;
					parentMap[x][y].x--;
					parentMap[x][y].y--;
				}
				else if (worldMap[i - 1][j] != 0 && worldMap[i][j - 1] == 0) {//左为海洋，上是陆地，那么它属于上边的国家
					int x = findParent(i, j-1).x;
					int y = findParent(i, j-1).y;
					//unionParent(x, y, i, j);
					parentMap[i][j].x = x;
					parentMap[i][j].y = y;
					parentMap[x][y].x--;
					parentMap[x][y].y--;
				}
				else {//如果左、上均为陆地，那么先将两个国家合并，再随意把它分给一个
					int x1 = findParent(i, j - 1).x;
					int y1 = findParent(i, j - 1).y;
					int x2 = findParent(i-1, j).x;
					int y2 = findParent(i-1, j).y;
					//cout << i<<" "<<j << " " <<x1 << " " << y1 << " " << x2 << " " << y2 << " " << endl;
					//cout << parentMap[x1][y1].x << " " << parentMap[x2][y2].x << endl;
					unionParent(x1,y1,x2,y2);
					//cout << parentMap[x1][y1].x << " " << parentMap[x2][y2].x << endl;
					//cout << "??" << endl;
					int x = findParent(i, j - 1).x;
					int y = findParent(i, j - 1).y;
					//cout << x << " " << y <<  " " << endl;
					//unionParent(x, y, i, j);
					parentMap[i][j].x = x;
					parentMap[i][j].y = y;
					parentMap[x][y].x--;
					parentMap[x][y].y--;
					//cout << "done" << endl;
				}
			}
		}
	}

	int countryNums = 0;
	for (int i = 1; i <= n;++i) {
		for (int j = 1; j <= m;++j) {
			if (parentMap[i][j].x<0) {
				++countryNums;
			}
		}
	}
	
	cout << countryNums;
	return 0;
}