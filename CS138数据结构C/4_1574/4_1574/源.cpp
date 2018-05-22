#include<iostream>
#pragma warning (disable:4996)
using namespace std;//输出应该是38
const int maxLongLongInt=6;//不能超过10
unsigned long long int whichProblem(unsigned long long int totalNum, unsigned long long int peopleNum, unsigned long long int addNumEachTime, unsigned long long int thisPeople) {
	unsigned long long int current = thisPeople;
	unsigned long long int actualNum = totalNum%peopleNum; //减少操作次数
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
	else {//减少操作次数
		const unsigned long long int tempflag = perTime%n; //n最多7次，所以longlongint要控制在10次左右
		unsigned long long int flag = tempflag;

		k -= maxLongLongInt;
		while (k > maxLongLongInt) {//这里如果取等，下面flag += (tempflag * power(10,k));就会误加(0次幂定义的原因)
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