#include<iostream>
#pragma warning (disable:4996)
using namespace std;
int mysteriousCode[101];
int sortArray[101] = {0};
int divide(int low,int high) {
	int tmp = mysteriousCode[low];//以首个元素做为排序的参考点
	while (low<high) {
		while (high>low && mysteriousCode[high]>=tmp) {
			--high;
		}
		mysteriousCode[low] = mysteriousCode[high];
		while (low<high && mysteriousCode[low] <= tmp) {
			++low;
		}
		if (high>low) { mysteriousCode[high] = mysteriousCode[low]; --high; }
	}
	mysteriousCode[low] = tmp;
	return low;
}
void quickSort(int low,int high) {
	if (low>=high) {
		return;
	}
	int mid = divide(low,high);
	quickSort(low, mid);
	quickSort(mid + 1, high);
}
int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int inputNum = 0, outputNum=0;
	cin >> inputNum;
	for (int i = 0; i < inputNum;++i) {
		cin >> mysteriousCode[i];
	}
	quickSort(0,inputNum-1);
	int j = 0;
	sortArray[0] = mysteriousCode[0];
	++outputNum;
	++j;
	for (int i = 1; i < inputNum;++i) {
		if (mysteriousCode[i] != mysteriousCode[i - 1]) {
			++outputNum;
			sortArray[j] = mysteriousCode[i];
			++j;
		}
	}
	cout << outputNum << endl;
	for (int i = 0; i < outputNum;++i) {
		cout << sortArray[i] << " ";
	}
	return 0;
}