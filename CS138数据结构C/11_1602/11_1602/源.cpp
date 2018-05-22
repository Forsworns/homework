#include<iostream>
#pragma warning (disable:4996)
using namespace std;
int firstArray[100000];
int secondArray[100000];
int mergeArray[200000];
void mergeSort(int first,int second) {
	int startFirst = 0, startSecond = 0,startMerge=0;
	while (startFirst != first&&startSecond!=second) {
		if (firstArray[startFirst] > secondArray[startSecond]) {
			mergeArray[startMerge] = secondArray[startSecond];
			startSecond++;
			startMerge++;
		}
		else {
			mergeArray[startMerge] = firstArray[startFirst];
			startFirst++;
			startMerge++;
		}
	}
	for (; startFirst < first;++startFirst,++startMerge) {
		mergeArray[startMerge] = firstArray[startFirst];
	}
	for (; startSecond < second;++startSecond,++startMerge) {
		mergeArray[startMerge] = secondArray[startSecond];
	}
}
int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int first, second;
	cin >> first >> second;
	for (int i = 0; i < first;++i) {
		cin >> firstArray[i];
	}
	for (int i = 0; i < second;++i) {
		cin >> secondArray[i];
	}
	mergeSort(first,second);
	for (int i = 0; i < first + second;++i) {
		cout << mergeArray[i] << " ";
	}
	return 0;
}