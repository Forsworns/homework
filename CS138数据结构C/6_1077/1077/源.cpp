#include<iostream>
using namespace std;
const int max = 31;
int tree[max]; //�洢��ʼ����
int preOrder[max][max]; //�洢ǰ��

int main() {
	int n = 0; //�����Ŀ<30
	int maxOrder = 0; //�洢��ǰ����е�λ��
	int maxScore = 0; //�洢�÷�
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