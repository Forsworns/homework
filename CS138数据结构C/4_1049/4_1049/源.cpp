#include<iostream>
#pragma warning (disable:4996)
using namespace std;
const int inStaion = -1; //在车站里停靠
const int gone = -2; //已经离开
/*template<class elemType>
class stack {
public:
	virtual bool isEmpty()const = 0;
	virtual void push(const elemType&x) = 0;
	virtual elemType pop() = 0;
	virtual elemType top() = 0;
	virtual ~stack() = 0;
};*/
template<class elemType>
class seqStack {
private:
	int maxSize;
	int top_p;
	elemType *elem;
	void doubleSpace();
public:
	seqStack(int initSize=10) {
		maxSize = initSize;
		elem = new elemType[initSize];
		top_p = -1;
	}
	bool isEmpty() {
		return top_p == -1;
	}
	bool isFull() {
		return top_p == maxSize - 1;
	}
	void clear() {
		top_p = -1;
	}
	void push(const elemType &x) {
		if (top_p == maxSize - 1) doubleSpace();
		elem[++top_p]=x;
	}
	elemType pop() {
		return elem[top_p--];
	}
	elemType top() {
		return elem[top_p];
	}
	int getTopNum() {
		return top_p;
	}
	~seqStack() {
		delete []elem;
	}
};
template<class elemType>
void seqStack<elemType>::doubleSpace() {
	elemType *temp;
	temp = new elemType[maxSize*2];
	for (int i = 0; i < maxSize;++i) {
		elem[i] = temp[i];
	}
	maxSize *= 2;
	delete []temp;
}
bool isPosible(int trainNum, seqStack<int> &station, int *testOrder, int *originOrder) { //利用originOrder记录状态
	for (int i = 0; i < trainNum;++i) {
		if (originOrder[testOrder[i]]==inStaion) {                               //看要离开的这个车testOrder[i]的状态，在站里先开出站再离开
			if (station.isEmpty()) return false;
			if (station.pop() != testOrder[i])return false;
			originOrder[testOrder[i]] = gone;
		}
		else {                                                        //不在站里直接离开
			originOrder[testOrder[i]] = gone;
		}

		for (int j = 0; j < testOrder[i];++j) {                      //检查在此车之前的车辆状态
			if (originOrder[j] !=inStaion&&originOrder[j] != gone) { //当一辆车想要离开，之前的车要么在站里要么已经走了
				if (!station.isFull()) {                             //所以他们不是这种状态的话要把他们变成这种状态，注意这里进站是顺序的
					station.push(originOrder[j]);
					originOrder[j] = inStaion;
				}
				else return false;                                    //如果车站已经满了，那么之前的未离开又未进站车辆就很尴尬说明错了
			}
		}
	}
	return true;
}
int main() {
	freopen("test.txt","r",stdin);
	int testTimes;
	int trainNum, trainContain;//火车数量是正数，但是火车容量可能为零,均小于等于1000
	int *testOrder,*originOrder;
	cin >> testTimes;
	
	for (int i = 0; i < testTimes;++i) {
		cin >> trainNum >> trainContain;
		testOrder = new int[trainNum];
		originOrder = new int[trainNum];
		seqStack<int> station(trainContain);
		
		for (int i = 0; i < trainNum;++i) {
			cin>>testOrder[i];
		}
		for (int i = 0; i < trainNum; ++i) {
			originOrder[i] = i;
		}
		if (isPosible(trainNum,station,testOrder,originOrder)) { //这里如果不是引用传递，还要析构一次导致无法获取到
			cout << "YES" << endl;
		}
		else cout << "NO" << endl;
	}
	
	return 0;
}