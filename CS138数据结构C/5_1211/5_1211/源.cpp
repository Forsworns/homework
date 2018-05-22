#pragma warning(disable:4996) //如果首先给出的结点中的子结点尚未定义怎么破，就是不是叶节点在先？
#include<iostream>
#include<cmath>
using namespace std;
class outOfBound {};
class illegalSize {};
const int nullFlag = 0;
int Height = 0;
int sheet[100000][2] = { 0 };
template <class elemType>
class vector {
private:
	elemType *data;
	int currentLength;
	int maxSize;
	void doubleSpace();
public:
	vector(int initSize=10) {
		if (initSize <= 0)throw illegalSize();
		data = new elemType[initSize];
		maxSize = initSize;
		currentLength = 0;
	}
	~vector() {
		delete[]data;
	}
	void clear() {
		currentLength = 0;
	}
	int size() const {
		return currentLength;
	}
	void push(elemType x);
	elemType visit(int i)const;
};
template<class elemType>
void vector<elemType>::doubleSpace() {
	elemType *tmp = data;
	maxSize *= 2;
	data = new elemType[maxSize];
	for (int i = 0; i < currentLength; ++i) {
		data[i] = tmp[i];
	}
	delete[]tmp;
 
}
template<class elemType>
void vector<elemType>::push(elemType x) {
	if (currentLength == maxSize) {
		doubleSpace();
	}
	data[currentLength++] = x;
}
template<class elemType>
elemType vector<elemType>::visit(int i)const {
	if (i<0 || i>currentLength - 1) {
		throw outOfBound();
	}
	return data[i];
}
vector<int> lastRow;
 
template<class elemType>
class treevector {
private:
	elemType *data;
	int currentLength;
	int maxSize;
	void doubleSpace() {
		elemType *tmp = data;
		maxSize *= 2;
		data = new elemType[maxSize];
		for (int i = 0; i < currentLength; ++i) {
			data[i] = tmp[i];
		}
		delete[]tmp;
	}
public:
	void setLength(int i) {
		currentLength = i;
	}
	treevector(int initSize = 100000) {
		if (initSize <= 0)throw illegalSize();
		data = new elemType[initSize];
		maxSize = initSize;
		currentLength = 0;
	}
	~treevector() {
		delete[]data;
	}
	void clear() {
		currentLength = 0;
	}
	int size() const {
		return currentLength;
	}
	void set(elemType& x, int order) {
		if (currentLength == maxSize) {
			doubleSpace();
		}
		data[order - 1] = x;
	}
	void push(elemType x) {
		if (currentLength == maxSize) {
			doubleSpace();
		}
		data[currentLength++] = x;
	}
	elemType& visit(int i)const {
		if (i<0) {
			throw outOfBound();
		}
		return data[i - 1];
	}
};
 
template<class Type>
class BinaryTree {
private:
	struct Node {
		Node *left, *right;
		Type data;
		int itsDeepth;
		Node() :left(NULL), right(NULL) {}
		Node(Type item, Node *L = NULL, Node *R = NULL) :data(item), left(L), right(R) {}
		~Node() {}
	};
	Node *root;
public:
	BinaryTree() :root(NULL) {}
	BinaryTree(const BinaryTree &another) {
		root = another.root;
	}
	BinaryTree(const Type &value) { root = new Node(value); }
	BinaryTree(const Node * p) { root = p; }
	//~BinaryTree() { clear(); }
	~BinaryTree() { }
	Type getRoot()const { return root->data; }
	Type getLeft()const { return root->left->data; }
	Type getRight()const { return root->right->data; }
 
	void makeTree(const Type &x, BinaryTree lt, BinaryTree rt) {
		root = new Node(x, lt.root, rt.root);
		lt.root = NULL;
		rt.root = NULL;
	}
	void createTree(const Type &i, treevector<BinaryTree<int> > &trees) { //递归，如果遇到需要处理的儿子，先去处理它。
		BinaryTree lt, rt;
		if (sheet[i - 1][0] != nullFlag) {
			createTree(sheet[i - 1][0], trees);
			lt = trees.visit(sheet[i - 1][0]);
		}
		if (sheet[i - 1][1] != nullFlag) { 
			createTree(sheet[i - 1][1], trees);
			rt = trees.visit(sheet[i - 1][1]);
		}
 
		/*if (sheet[i - 1][0] != nullFlag) { //visit函数返回问题
			lt = trees.visit(sheet[i - 1][0]);
		}
		if (sheet[i - 1][1] != nullFlag) {
			rt = trees.visit(sheet[i - 1][1]);
		}*/
		makeTree(i, lt, rt);
		trees.set(*this, i );
	}
	void delLeft() {
		BinaryTree tmp = root->left;
		root->left = NULL;
		tmp.clear();
	}
	void delRight() {
		BinaryTree tmp = root->right;
		root->right = NULL;
		tmp.clear();
	}
	bool isEmpty() {
		return root == NULL;
	}
	void clear() {
		if (root != NULL) {
			clear(root);
			root = NULL;
		}
	}
	int size() const {
		return size(root);
	}
	int height() const {
		return height(root);
	}
	bool isBinary() {
		if (root != NULL) {
			int heightnum = height(root) - 1;
			int number = 0;
			if (heightnum>=1) {
				if (preOrder(root, Height, heightnum, number) != pow(2, heightnum - 1)) {
					return false;
				}
			}
			int status = 2; //记录最后一行状态全是1的话为2
			for (int i = 0; i < lastRow.size(); ++i) { //有零存在是1
				if (status == 2 && !lastRow.visit(i)) {
					status = 1;
				}
				if (status == 1 && lastRow.visit(i)) {//如果有一个1在零后面那么状态就置为0
					status = 0;
				}
			}
			return status>0;
		}
		return true;
	}
private:
	int height(Node *t) const {
		if (t == NULL) { return 0; }
		else {
			int lt = height(t->left), rt = height(t->right);
			return 1 + ((lt > rt) ? lt : rt);
		}
	}
	void clear(Node *t) {
		if (t->left != NULL) {
			clear(t->left);
		}
		if (t->right != NULL) {
			clear(t->right);
		}
		delete t;
	}
	int size(Node *t)const {
		if (t == NULL) { return 0; }
		return 1 + size(t->left) + size(t->right);
	}
	int preOrder(Node *t, int &Height, int &heightnum, int &number)const { //前序遍历设置各个节点的深度,heightnum是总高度，number是倒数第二行的结点数目
		if (t != NULL) {
			Height++; //下降一层
					  //cout << t->data << ' ';
					  //cout << Height << endl;
			t->itsDeepth = Height;
			if (Height == heightnum) { //统计倒数第二行的结点数目，并记录最后一行的情况
				number++;
				if (t->left) {
					lastRow.push(1);
				}
				else {
					lastRow.push(0);
				}
				if (t->right) {
					lastRow.push(1);
				}
				else {
					lastRow.push(0);
				}
			}
			preOrder(t->left, Height, heightnum, number);
			preOrder(t->right, Height, heightnum, number);
			Height--; //返回一层
		}
		return number; //返回倒数第二行的结点数目
	}
};
 
int main() {
	//freopen("test.txt", "r", stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int N, leftSonData, rightSonData;
	treevector<BinaryTree<int> > trees;
	int sizemax = 0,maxPosition = 0;
	BinaryTree<int> smallTree;
	cin >> N;
	trees.setLength(N);
	for (int i = 0; i < N; ++i) {
		cin >> leftSonData >> rightSonData;
		sheet[i][0] = leftSonData;
		sheet[i][1] = rightSonData;
	}
	for (int i = 1; i <= N; ++i) {
		smallTree.createTree(i,trees);
	}
	for (int i = 1; i <= N; ++i) {
		if (sizemax < trees.visit(i).size()) {
			sizemax = trees.visit(i).size();
			maxPosition = i;
		}
	}
	if (trees.visit(maxPosition).isBinary()) {
		cout << "Y" << endl;
	}
	else {
		cout << "N" <<endl;
	}
	/*smallTree.makeTree(4, trees.visit(2), trees.visit(1));
	cout << smallTree.size() << endl;*/
	//cout << trees.visit(1).size() << endl;
	/*for (int i = 0; i < lastRow.size();++i) {
		cout << lastRow.visit(i);
	}*/
	return 0;
}
 
 
 
/*template<class elemType> 石乐志用链表应该用向量
class linkList{
private:
struct listNode{
listNode *next=NULL;
elemType data;
listNode(){}
listNode(elemType pushData) {
data = pushData;
}
};
listNode *move(int order)const{
listNode *temp = head->next;
while (order--) {
temp = temp->next;
}
return  temp;
}
listNode *head, *tail;
int length;
public:
linkList() {
head = new listNode;
head->next = tail = new listNode;
length = 0;
}
void push(elemType pushData) {
listNode *temp = new listNode(pushData);
listNode *prev = move(length);
prev->next = temp;
temp->next = tail;
++length;
}
elemType find(int order) {
if (order > length) { throw outOfBound(); }
if (order == nullFlag) {
return nullTree;
}
listNode *temp = move(order);
return  temp->data;
}
};*/
 
 
 
/*void createTree(Type flag) { //flag表示空结点
linkQueue<Node*>que;
Node *tmp;
Type x, ldata, rdata;
cin >> x;
root = new Node(x);
que.enQueue(root);
while (!que.isEmpty()) {
tmp = que.deQue();
cin >> ldata >> rdata;
if (ldata != flag) {
que.enQueue(tmp->left = new Node(ldata));
}
if (rdata != flag) {
que.enQueue(tmp->right = new Node(rdata));
}
}
}*/