#include<iostream>
#pragma warning (disable:4996)
using namespace std;
int number[105] = { 0 };
int exist[101] = { 0 };
int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int amount;
	cin >> amount;
	for (int i = 1; i <= amount;++i) {
		cin >> number[i];
		exist[number[i]] = 1;
	}

	for (int i = 1; i <= amount;++i) {
		for (int j = 1; j <= amount;++j) {
			if (number[i]>number[j]) {//�����������ظ���
				int k = number[i] / number[j];
				if (!exist[k]) {
					number[++amount] = k;
					exist[k] = 1;
				}
			}
			else if(number[i]<number[j]){
				int k = number[j] / number[i];
				if (!exist[k]) {
					number[++amount] = k;
					exist[k] = 1;
				}
			}
		}
	}
	cout << amount;//����amount�����ܵ������ˣ����Ǵ�ͷͳ��exist[i]Ϊ1��ȴ���ԣ���
	return 0;
}