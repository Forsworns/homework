#include<iostream>
#include<vector>
#pragma warning (disable:4996)
using namespace std;
int bakeup[1000001];
int input[1000001];
class dp {
public:
	int amount = 0;
	int last = 0;
	dp(int a, int b) :amount(a), last(b) {}
	dp() :amount(0), last(0) {}
};
dp dpList[1000001];
int dpLast[1000001] = { 0 };
int binarySearch(int in, int start, int end) {
	if (start == end) {
		if (dpLast[start] >= in) { //这里是取得下界lower_bound，不加等于是上界upper_bound
			return start;
		}
		else {
			return start + 1;
		}
	}
	int mid = (start + end) / 2;
	if (dpLast[mid] >= in) {   //同样这里是取得下界lower_bound，不加等于是上界upper_bound
		binarySearch(in, start, mid);
	}
	else {
		binarySearch(in, mid + 1, end);
	}
}
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> input[i];
	}
	for (int i = 0; i < n; ++i) {
		bakeup[i] = -input[i];
	}
	int length = 1;
	dpLast[0] = bakeup[0];
	for (int i = 1; i < n; ++i) {
		if (dpLast[length - 1]<bakeup[i]) {  //可以打下来的是求最长递减子序列，也就是相反数的最长递增子序列
			//cout << length << endl;
			dpLast[length++] = bakeup[i];
		}
		else {
			int pos = binarySearch(bakeup[i], 0, length - 1); //这里最开始写成了i-1
															 //cout << pos<<endl;
			dpLast[pos] = bakeup[i];
		}
	}
	cout << length << endl;;

	length = 1;
	dpLast[0] = input[0];
	for (int i = 1; i < n; ++i) { //导弹数目使用LIS即可
		if (dpLast[length - 1]<input[i]) {
			//cout << length << endl;
			dpLast[length++] = input[i];
		}
		else {
			int pos = binarySearch(input[i], 0, length - 1); //这里最开始写成了i-1
															 //cout << pos<<endl;
			dpLast[pos] = input[i];
		}
	}
	cout << length;

	return 0;
}