#include<iostream>
using namespace std;
const int max = 31;
int tree[max]; //存储初始输入
int preOrder[max][max]; //存储前序

int main() {
	int n = 0; //结点数目<30
	int maxOrder = 0; //存储在前序表中的位置
	int maxScore = 0; //存储得分
	cin >> n;
	for (int i = 0; i < n;++i) {
		cin >> tree[i];
	}
	cout << maxScore << endl;
	for (int i = 0; i < n; ++i) {
		cout << preOrder[maxOrder][i] << " ";
	}

	return 0;
}