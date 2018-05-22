#pragma warning(disable:4996)
#include<iostream>
#include<cstdio>
using namespace std;
int findBiggerNum(long long int y, long long int x[], int xLength, int num) { //���ƶ��ַ����� �ݹ�
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
/*int findBiggerNum(long long int y, long long int x[], int xLength, int num) { //ѭ����ʽ
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
		if (y[i]>y[i - 1]) { answer[i] = findBiggerNum(y[i], x + N - answer[i - 1], answer[i - 1], answer[i - 1]); } //�˴�����֮ǰ�Ľ��������֮������㣬Ϊɶ���ԣ���
		else answer[i] = findBiggerNum(y[i], x, N, N);																									//˵�����Ĳ������ݵڶ������鲻���Ѿ��ź����
	}
	for (int i = 0; i < T; ++i) { //coutЧ�ʵ��˷�ʱ��
		printf("%d",answer[i]);
		printf("\n");
		//cout << answer[i] << endl;
	}
	return 0;
}