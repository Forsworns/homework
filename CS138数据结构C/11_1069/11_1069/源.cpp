#include<iostream>
#include<algorithm>
#pragma warning (disable:4996)
using namespace std;
int probableMoneyNums, maxMoney, varietyNum;
int moneyValue[1000] = { 0 }, coinNums[105] = { 0 }, dpProbableMoney[100005] = { 0 };

int main() {
	freopen("test.txt", "r", stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	cin >> varietyNum >> maxMoney;
	while (varietyNum != 0 && maxMoney != 0) {
		probableMoneyNums = 0;
		for (int i = 1; i <= varietyNum;++i) {
			cin >> moneyValue[i];
		}
		for (int i = 1; i <= varietyNum; ++i) {
			cin >> coinNums[i];
		}									//���б�������ղ����������ղؼ���	
		for (int i = 1; i <= varietyNum;++i) {
			int j = 1;
			for (; j <= coinNums[i] / 2;j=j*2) {//���ö����Ƽ�֦�����ӶȽ�����
				for (int k = maxMoney; k >= j*moneyValue[i];--k) {	//С��k*moneyValue��һ���Ų���Ӳ�ң��Ͳ�������
					dpProbableMoney[k] = max(dpProbableMoney[k], dpProbableMoney[k - j*moneyValue[i]] + j*moneyValue[i]);
				}
			}

			j = coinNums[i] - j + 1;//��ΪҪ���ö����Ƶ�˼��ȥ��ʾ��ʾ����ô�����ڶ��������ͣ�����9��1/2/4/8������ѡ��8��Ҫ��1/2/4/(9-1-2-4)
			for (int k = maxMoney; k >= j*moneyValue[i]; --k) {	//С��k*moneyValue��һ���Ų���Ӳ�ң��Ͳ�������
				dpProbableMoney[k] = max(dpProbableMoney[k], dpProbableMoney[k - j*moneyValue[i]] + j*moneyValue[i]);
			}
		}
		for (int i = 1; i <= maxMoney; ++i) {//01�����Ķ�̬�滮��״̬ת�Ʒ��̵Ķ�����i�ռ�ı����ܷ��µ����ļ�ֵ�����������Ƚ������ֵ�Ϳռ���ͬ������ֻ��Ҫ��һ�µ�ǰ�Ƿ�������Ϳ����ж���û��������ˡ�
			if (dpProbableMoney[i]==i) {
				++probableMoneyNums;
			}
		}
		cout << probableMoneyNums << endl;
		for (int i = 1; i <= maxMoney;++i) {
			dpProbableMoney[i] = 0;
		}
		cin >> varietyNum >> maxMoney;
	}
	return 0;
}