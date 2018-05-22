#pragma warning(disable:4996)
#include<iostream>
using namespace std;
/*template<class elemType>
class stack {
public:
	virtual bool isEmpty()const = 0;
	virtual void push(const elemType &x) = 0;
	virtual elemType pop() = 0;
	virtual elemType top()const = 0;
	virtual ~stack(){}
};*/
template<class elemType> //题目可能第二个数组它不是顺序排列的，这个没法用了
class seqStack {
private:
	elemType *elem;
	int top_p;
	int maxSize;
	void doubleSpace();
public:
	seqStack(int initSize=10) {
		elem = new elemType[initSize];
		maxSize = initSize;
		top_p = -1;
	}
	int maxSizeNum()const {
		return maxSize;
	}
	int topNum() {
		return top_p;
	}
	bool isEmpty()const {
		return top_p == -1;
	}
	void push(const elemType &x) {
		if (top_p==maxSize-1) doubleSpace();
		elem[++top_p] = x;
	}
	elemType pop() {
		return elem[top_p--];
	}
	elemType top()const {
		return elem[top_p];
	}
	~seqStack() { delete[]elem; }
};
template<class elemType>
void seqStack<elemType>::doubleSpace() {
	elemType *temp = elem;
	elem = new elemType[maxSize*2];
	maxSize *= 2;
	for (int i = 0; i < maxSize;++i) {
		elem[i] = temp[i];
	}
	delete []temp;
}
int main() {
	freopen("test.txt","r",stdin);
	long long int *x, *y;
	seqStack<int> allNum(100000);
	seqStack<int> loveNum(100000);
	int N, T;
	cin >> N >> T;
	x = new long long int[N];
	y = new long long int[T];
	for (int i = 0; i < N; ++i) {
		cin >> x[i];
		allNum.push(x[i]);
	}
	for (int i = 0; i < T;++i) {
		cin >> y[i];
		loveNum.push(y[i]);
	}
	for (int i = 0; i < T;++i) {
		int temp = loveNum.pop();
		while (allNum.top()>temp) { //当手中的数的栈顶元素大于二哥喜欢的数的时候，手中数出栈
			allNum.pop();
		}
		y[T-i-1]= N - allNum.topNum() - 1 ;//那么大于喜欢数的个数就是已经出栈的数量（因为喜欢的数已经排序过了，所以底下的一定比上面的小）
	}
	for (int i = 0; i < T; ++i) {
		cout << y[i]<<endl;
	}
	return 0;
}