#include<iostream>
#include<cstring>
#pragma warning (disable:4996)
using namespace std;
char tree[1000001];
int testNum = 0;
int treeNodesNum = 0;
char order[10];
char codeType[10];
const char ENCODE[10] = "ENCODE";
const char DECODE[10] = "DECODE";
const char PREORDER[10] = "PREORDER";
const char POSORDER[10] = "POSORDER";
const char INORDER[10] = "INORDER";
template<class elemType> //��Ŀ���ܵڶ�������������˳�����еģ����û������
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

void encodePre() { //����һ�·ǵݹ鷽��
	for (int j = 0; j<treeNodesNum; ++j) {
		cin >> tree[j];
	}
	int root=0,left,right;
	seqStack<int> tmp;
	tmp.push(root);
	while (!tmp.isEmpty()) {
		root = tmp.pop();
		cout << tree[root];
		left = root * 2 + 1;
		right = root * 2 + 2;
		if (right<treeNodesNum) { tmp.push(right); }
		if (left<treeNodesNum) { tmp.push(left); }
	}
	
}

void enIn(int root) {
	if (root * 2 + 1<treeNodesNum) {
		enIn(root * 2 + 1);
	}
	cout << tree[root];
	if (root * 2 + 2 < treeNodesNum) {
		enIn(root * 2 + 2);
	}
}
void encodeIn() {
	for (int j = 0; j<treeNodesNum; ++j) {
		cin >> tree[j];
	}
	int root = 0;
	enIn(root);
}

void enPos(int root) {
	if (root * 2 + 1<treeNodesNum) {
		enPos(root * 2 + 1);
	}
	if (root * 2 + 2 < treeNodesNum) {
		enPos(root * 2 + 2);
	}
	cout << tree[root];
}
void encodePos() {
	for (int j = 0; j<treeNodesNum; ++j) {
		cin >> tree[j];
	}
	int root = 0;
	enPos(root);
} //�������ɲ������ǰ���

void LevelOut() {
	for (int i = 0; i < treeNodesNum;++i) {
		cout << tree[i];
	}
}

void makeTreePre(int root) {
	cin >> tree[root];
	if (root * 2 + 1<treeNodesNum) {
		makeTreePre(root * 2 + 1);
	}
	if (root * 2 + 2 < treeNodesNum) {
		makeTreePre(root * 2 + +2);
	}
}
void decodePre() {
	int root=0;
	makeTreePre(root);
	LevelOut();
}//��������ǰ���������,�������������ֱ�������±�ȥ���룬����ͨ������Ȼ������ķ���ȥʵ��

void makeTreeIn(int root) {
	if (root * 2 + 1<treeNodesNum) {
		makeTreeIn(root * 2 + 1);
	}
	cin >> tree[root];
	if (root * 2 + 2 < treeNodesNum) {
		makeTreeIn(root * 2 + +2);
	}
}
void decodeIn() {
	int root = 0;
	makeTreeIn(root);
	LevelOut();
}

void makeTreePos(int root) {
	if (root * 2 + 1<treeNodesNum) {
		makeTreePos(root * 2 + 1);
	}
	if (root * 2 + 2 < treeNodesNum) {
		makeTreePos(root * 2 + +2);
	}
	cin >> tree[root];
}
void decodePos() {
	int root = 0;
	makeTreePos(root);
	LevelOut();
}
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	freopen("test.txt", "r", stdin);
	cin >> testNum;
	for (int i = 0; i < testNum; ++i) {
		cin >> treeNodesNum;
		cin >> order;
		cin >> codeType;
		if (strcmp(codeType, ENCODE) == 0) {
			if (strcmp(order, INORDER) == 0) {
				encodeIn();
			}
			else if (strcmp(order, PREORDER) == 0) {
				encodePre();
			}
			else {
				encodePos();
			}
		}
		else {
			if (strcmp(order, INORDER) == 0) {
				decodeIn();
			}
			else if (strcmp(order, PREORDER) == 0) {
				decodePre();
			}
			else {
				decodePos();
			}
		}
		cout << endl;
	}
	return 0;
}