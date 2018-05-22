#include<iostream>
using namespace std;
#pragma warning (disable:4996)
char inputFirst[1001];
char inputSecond[1001];
int dpLCS[1001][1001] = {0};
int main() {
	freopen("test.txt","r",stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int first,second,answer=0;
	cin >> first>>second;
	for (int i = 0; i < first;++i) {
		cin >> inputFirst[i];
	}
	for (int i = 0; i < second;++i) {
		cin >> inputSecond[i];
	}
	for (int i = 1; i <= first;++i) {
		for (int j = 1; j <= second;++j) { //ע���һ�С�����Ĭ�ϵ�0
			if (inputFirst[i-1]==inputSecond[j-1]) {
				dpLCS[i][j] = dpLCS[i - 1][j - 1]+1;
			}
			else {
				dpLCS[i][j] = dpLCS[i - 1][j]>dpLCS[i][j - 1] ? dpLCS[i - 1][j] : dpLCS[i][j - 1];
			} //�õ�i��j�е����ֱ�ʾǰi,j���ַ�������ͬ���еĸ���
		}
	}
	cout << dpLCS[first][second];
	return 0;
}