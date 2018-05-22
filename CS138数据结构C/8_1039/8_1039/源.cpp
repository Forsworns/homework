#include<iostream>
#pragma warning (disable:4996)
using namespace std; //以为是堆栈溢出，结果是注意层数15层限制存储空间，不是节点数n
int pos[80000] = {0};
int tree[80000] = {0};
struct node {
	int data;
	int timesPop;
	node(int t = 0) :data(t),timesPop(0){ }
};
template<class elemType>
class seqStack {
private:
	elemType *elem;
	int top_p;
	int maxSize;
	void doubleSpace();
public:
	seqStack(int initSize = 10) {
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
		if (top_p == maxSize - 1) doubleSpace();
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
	elem = new elemType[maxSize * 2];
	maxSize *= 2;
	for (int i = 0; i < maxSize; ++i) {
		elem[i] = temp[i];
	}
	delete[]temp;
}

/*void posOrder(int root) { //递归
	if (tree[root * 2]!=0) {
		posOrder(root * 2);
	}
	if (tree[root * 2 + 1] != 0) {
		posOrder(root * 2 + 1);
	}
	if (tree[root]!=0) {
		cout << tree[root]<<" ";
	}
	return;
}*/
void posOrder(int root) { //非递归后序要三次出栈
	seqStack<node> s;
	node current(root);
	s.push(current);
	while (!s.isEmpty()) {
		current = s.pop();
		if(++current.timesPop == 3){
			cout << tree[current.data]<< " ";
			continue;
		}
		s.push(current);
		if (current.timesPop == 1) {
			if (tree[current.data*2]!=0) {
				s.push(node(current.data*2));
			}
		}
		else {
			if (tree[current.data * 2+1] != 0) {
				s.push(node(current.data*2+1));
			}
		}
	}
}
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	freopen("test.txt", "r", stdin);
	int num;
	int self, leftson, rightson;
	pos[1]=1;
	tree[1]=1;
	cin >> num;
	for (int i = 1; i <= num;++i) {
		cin >> self >> leftson >> rightson;
		if (leftson!=-1) {
			tree[pos[self]*2] = leftson;
			pos[leftson] = pos[self] * 2;
		}
		if (rightson!=-1) {
			tree[pos[self] *2+1] = rightson;
			pos[rightson] = pos[self] * 2+1;
		}
	}
	for (int i = 1; i <= num; ++i) {
		cout << pos[i] << " ";
	}
	cout << endl;
	posOrder(1);
	return 0;
}