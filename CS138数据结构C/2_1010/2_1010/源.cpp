#include<iostream>
using namespace std;
const int salary = 300;
const int monthNum = 12;
int main() {
	int brotherMoney = 0, motherMoney = 0, hundredNums = 0;//���������Ǯ��������������Ǯ��100Ԫ������
	int budget[monthNum] = { 0 };
	bool moneyEnough = true;
	for (int i = 0; i < monthNum; ++i) {
		cin >> budget[i];
	}
	for (int j = 0; j < monthNum; ++j) {
		brotherMoney += salary;//�³���Ǯ
		brotherMoney -= budget[j];
		if (brotherMoney < 0) {
			cout << - (j + 1) << endl;
			moneyEnough = false;
			break;
		}
		hundredNums = brotherMoney / 100;//100����Ŀ���Ķ����Ǯ��λ
		brotherMoney -= 100 * hundredNums;
		motherMoney += 100 * hundredNums;
	}
	if (moneyEnough) cout << motherMoney*1.2+brotherMoney << endl;
	return 0;
}