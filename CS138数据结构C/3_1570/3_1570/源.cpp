#pragma warning(disable:4996)
#include<iostream>
#include<cstdio>
using namespace std;
int findBiggerNum(long long int y, long long int x[], int xLength, int num) { //类似二分法查找 递归
	if (xLength == 0) {
		return num;
	}
	if (xLength == 1) {
		if (x[0] <= y) {
			--num;
		}
		return num;
	}
	int xLengthHalf = xLength / 2;
	if (x[xLengthHalf - 1] == y) {
		return num - xLengthHalf;
	}
	else if (x[xLengthHalf - 1]<y) {
		return findBiggerNum(y, x + xLengthHalf, xLength - xLengthHalf, num - xLengthHalf);
	}
	else {
		return findBiggerNum(y, x, xLengthHalf - 1, num);
	}
}
/*int findBiggerNum(long long int y, long long int x[], int xLength, int num) { //循环形式
while (xLength>1) {
int xLengthHalf = xLength / 2;
if (x[xLengthHalf - 1] == y) {
return num - xLengthHalf;
}
else if (x[xLengthHalf - 1]<y) {
x += xLengthHalf;
xLength -= xLengthHalf;
num -= xLengthHalf;
}
else {
xLength = xLengthHalf - 1;
}
}
if (xLength == 1) {
if (x[0] <= y) {
--num;
}
return num;
}
if (xLength == 0) {
return num;
}
}*/

int main() {
	//freopen("test.txt","r",stdin);
	long long int x[100000], y[100000];
	int answer[100000];
	int N, T;
	cin >> N >> T;
	/*int *answer;
	long long int *x, *y;
	x = new long long int[N];
	y = new long long int[T];
	answer = new int[T];*/
	for (int i = 0; i < N; ++i) {
		//scanf("%I64d",x[i]);
		cin >> x[i];
	}

	for (int i = 0; i < T; ++i) {
		//scanf("%I64d", x[i]);
		cin >> y[i];
	}
	answer[0] = findBiggerNum(y[0], x, N, N);
	for (int i = 1; i < T; ++i) {
		if (y[i]>y[i - 1]) { answer[i] = findBiggerNum(y[i], x + N - answer[i - 1], answer[i - 1], answer[i - 1]); } //此处利用之前的结果来进行之后的运算，为啥不对？？
		else answer[i] = findBiggerNum(y[i], x, N, N);																									//说明给的测试数据第二个数组不是已经排好序的
	}
	for (int i = 0; i < T; ++i) { //cout效率低浪费时间
		printf("%d",answer[i]);
		printf("\n");
		//cout << answer[i] << endl;
	}
	return 0;
}