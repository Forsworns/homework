#include<iostream>
#pragma warning (disable:4996)
using namespace std;
const int inStaion = -1; //�ڳ�վ��ͣ��
const int gone = -2; //�Ѿ��뿪
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
bool isPosible(int trainNum, seqStack<int> &station, int *testOrder, int *originOrder) { //����originOrder��¼״̬
	for (int i = 0; i < trainNum;++i) {
		if (originOrder[testOrder[i]]==inStaion) {                               //��Ҫ�뿪�������testOrder[i]��״̬����վ���ȿ���վ���뿪
			if (station.isEmpty()) return false;
			if (station.pop() != testOrder[i])return false;
			originOrder[testOrder[i]] = gone;
		}
		else {                                                        //����վ��ֱ���뿪
			originOrder[testOrder[i]] = gone;
		}

		for (int j = 0; j < testOrder[i];++j) {                      //����ڴ˳�֮ǰ�ĳ���״̬
			if (originOrder[j] !=inStaion&&originOrder[j] != gone) { //��һ������Ҫ�뿪��֮ǰ�ĳ�Ҫô��վ��Ҫô�Ѿ�����
				if (!station.isFull()) {                             //�������ǲ�������״̬�Ļ�Ҫ�����Ǳ������״̬��ע�������վ��˳���
					station.push(originOrder[j]);
					originOrder[j] = inStaion;
				}
				else return false;                                    //�����վ�Ѿ����ˣ���ô֮ǰ��δ�뿪��δ��վ�����ͺ�����˵������
			}
		}
	}
	return true;
}
int main() {
	freopen("test.txt","r",stdin);
	int testTimes;
	int trainNum, trainContain;//�����������������ǻ���������Ϊ��,��С�ڵ���1000
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
		if (isPosible(trainNum,station,testOrder,originOrder)) { //��������������ô��ݣ���Ҫ����һ�ε����޷���ȡ��
			cout << "YES" << endl;
		}
		else cout << "NO" << endl;
	}
	
	return 0;
}