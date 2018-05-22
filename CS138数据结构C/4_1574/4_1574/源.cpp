#include<iostream>
#pragma warning (disable:4996)
using namespace std;//���Ӧ����38
const int maxLongLongInt=6;//���ܳ���10
unsigned long long int whichProblem(unsigned long long int totalNum, unsigned long long int peopleNum, unsigned long long int addNumEachTime, unsigned long long int thisPeople) {
	unsigned long long int current = thisPeople;
	unsigned long long int actualNum = totalNum%peopleNum; //���ٲ�������
	for (int i = 0; i < actualNum;++i) {
		current = (current + addNumEachTime) % peopleNum;
	}
	return current;
}
unsigned long long int power(unsigned long long int baseNum, int indexNum) {
	if (indexNum==0) {
		return 1;
	}
	if (indexNum==1) {
		return baseNum;
	}
	else return baseNum*power(baseNum,indexNum-1);
}
int main() {
	unsigned long long int k, m, n,x;
	const unsigned long long int perTime = power(10, maxLongLongInt);
	//freopen("test.txt","r",stdin);
	int flag = 0;
	cin >> n >> m >> k >> x;
	if (k<=maxLongLongInt) {
		x = whichProblem(power(10, k), n, m, x);
		cout << x << endl;
	}
	else {//���ٲ�������
		const unsigned long long int tempflag = perTime%n; //n���7�Σ�����longlongintҪ������10������
		unsigned long long int flag = tempflag;

		k -= maxLongLongInt;
		while (k > maxLongLongInt) {//�������ȡ�ȣ�����flag += (tempflag * power(10,k));�ͻ����(0���ݶ����ԭ��)
			flag *= perTime;
			flag %= n;
			k -= maxLongLongInt;
		} 
		flag *= power(10, k);

		x = whichProblem(flag, n, m, x);
		cout << x << endl;
	}
	return 0;
}