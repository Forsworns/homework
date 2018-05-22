#include<iostream>
#include<cstdio>
using namespace std;
int field[1001][1001]={0}; //����ȡ��һ��һ���Ա�ס��������
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
            field[i][j]=tmp+field[i-1][j]+field[i][j-1]-field[i-1][j-1]; //�洢���󶥵㵽�õ���Ӿ����Ԫ�صĺʹ�С
		}
	}
	cin >> fixRow >> fixCol; //��ȡ�Ӿ���Ĵ�С
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