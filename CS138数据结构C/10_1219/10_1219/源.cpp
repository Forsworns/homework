#include<iostream>
#include<algorithm>	//妈耶应该用long long int 没有估计好范围
#pragma warning (disable:4996)
using namespace std;
int arrayInput[200000] = { 0 };
int tmp[200000] = { 0 };
long long int getAmount(int start,int end,int x,int arrayInput[]) {  //递增开区间序列，统计小于该数的数量
	int pos;
	int tmpStart = start;
	while (start<end) {
		int mid = (start+end) / 2;
		if (2*arrayInput[mid]<x) {
			start = mid + 1;
			pos = start;
		}
		else {
			end = mid;
			pos = mid;
		}
	}
	
	return pos-tmpStart;
}
long long int mergeArrayAdd(int input[],int firstPos,int mid,int lastPos,int tmpSort[]) {
	long long int count=0;
	int frontStart = firstPos;
	int frontEnd = mid;
	int backStart = mid + 1;
	int backEnd = lastPos;
	int tmpSortPos = lastPos;
	while (frontStart<=frontEnd&&backStart<=backEnd) { //先归并排序，遇到逆序的再找是不是重要逆序
		if (input[frontEnd]<=input[backEnd]) {
			tmpSort[tmpSortPos--] = input[backEnd--];
		}
		else { //找到逆序数，判断是不是重要逆序，如果是，由于已经排成了顺序，那么后半个数组的该元素前面的都是
			count = count + getAmount(backStart, backEnd + 1, input[frontEnd],input); //这里findPos是开区间相当于upper_bound
			/*cout <<"起点" <<backStart << "终点"<<backEnd + 1 << "比较值"<<input[frontEnd];
			for (int i = backStart; i <= backEnd; ++i) {
				cout << " " << input[i] << " ";
			}
			cout << "结果"<<getAmount(backStart, backEnd + 1, input[frontEnd],input) << endl;*/
			tmpSort[tmpSortPos--] = input[frontEnd--];  //原理是你把左右各自排成顺序，并不影响左右相对的重要逆序关系，之前想复杂了，左右不排成顺序不好做
			
		}
	}
	while (frontStart<=frontEnd) {
		tmpSort[tmpSortPos--] = input[frontEnd--];
	}
	while(backStart<=backEnd) {
		tmpSort[tmpSortPos--] = input[backEnd--];
	}
	for (int i = firstPos; i <= lastPos;++i) {
		input[i] = tmpSort[i];
	}
	return count;
}
long long int  mergeSort(int input[],int firstPos,int lastPos,int tmpSort[]) {
	if (firstPos>=lastPos) {
		return 0;
	}
	int mid = (firstPos+lastPos) / 2;
	long long int left = mergeSort(input,firstPos,mid,tmpSort);
	long long int right = mergeSort(input,mid+1,lastPos,tmpSort);
	long long int all = mergeArrayAdd(input,firstPos,mid,lastPos,tmpSort);
	return (left + right + all);
}
int main() {
	freopen("test.txt","r",stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int n;
	cin >> n;
	for (int i = 0; i < n;++i) {
		cin >> arrayInput[i];
	}
	/*for (int i = 0; i < n; ++i) {
		cout<< arrayInput[i];
	}*/
	cout<<mergeSort(arrayInput, 0,n-1,tmp);
	return 0;
}