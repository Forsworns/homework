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
		}									//还有别的做法收藏起来了在收藏夹里	
		for (int i = 1; i <= varietyNum;++i) {
			int j = 1;
			for (; j <= coinNums[i] / 2;j=j*2) {//利用二进制剪枝将复杂度降下来
				for (int k = maxMoney; k >= j*moneyValue[i];--k) {	//小于k*moneyValue的一定放不下硬币，就不考虑了
					dpProbableMoney[k] = max(dpProbableMoney[k], dpProbableMoney[k - j*moneyValue[i]] + j*moneyValue[i]);
				}
			}

			j = coinNums[i] - j + 1;//因为要利用二进制的思想去表示表示，那么倒数第二个拆分项就：比如9有1/2/4/8，不能选用8而要用1/2/4/(9-1-2-4)
			for (int k = maxMoney; k >= j*moneyValue[i]; --k) {	//小于k*moneyValue的一定放不下硬币，就不考虑了
				dpProbableMoney[k] = max(dpProbableMoney[k], dpProbableMoney[k - j*moneyValue[i]] + j*moneyValue[i]);
			}
		}
		for (int i = 1; i <= maxMoney; ++i) {//01背包的动态规划的状态转移方程的定义是i空间的背包能放下的最大的价值，但是这道题比较特殊价值和空间相同，所以只需要看一下当前是否等于最大就可以判断有没有这个数了。
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