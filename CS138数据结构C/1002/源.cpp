#include<iostream>
#include<cstdio>
using namespace std;
int field[1001][1001]={0}; //故意取大一行一列以保住符合条件
int row = 1, col = 1;
int fixRow, fixCol;

int main() {
    std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	cin >> row >> col;
    int tmp=0;
	for (int i = 1; i <= row; ++i) {
		for (int j = 1; j <= col; ++j) {
			cin >> tmp;
            field[i][j]=tmp+field[i-1][j]+field[i][j-1]-field[i-1][j-1]; //存储从左顶点到该点的子矩阵的元素的和大小
		}
	}
	cin >> fixRow >> fixCol; //求取子矩阵的大小
	int answer = 0;
	int midBus = 0;
	for (int i = fixRow; i <= row; ++i) {
		for (int j = fixCol; j <= col; ++j) {
			midBus = field[i][j]-field[i-fixRow][j]-field[i][j-fixCol]+field[i-fixRow][j-fixCol];
			answer = (midBus > answer) ? midBus : answer;
		}
	}
	cout << answer;
	return 0;
}