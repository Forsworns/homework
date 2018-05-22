#pragma warning(disable:4996)
#include<iostream>
#include<cmath>
using namespace std;
class outOfBound {};
class illegalSize {};
const int nullFlag = 0;
//int Height = 0; //�Ƕ�����ת���ɶ����������
int TrueHeight = 1;//��ʵ��ȣ�Ҳ������������ȣ�����������ʵ���ֵܣ�������������
int sheet[100000][3] = { 0 };
int createFlag[100000] = { 0 }; //���ٵݹ���ٶ�ջ�������
int brothersShown[100000] = {0};
template <class elemType>
class vector {
private:
	elemType *data;
	int currentLength;
	int maxSize;
	void doubleSpace();
public:
	vector(int initSize = 10) {
		if (initSize <= 0)throw illegalSize();
		data = new elemType[initSize];
		maxSize = initSize;
		currentLength = 0;
	}
	~vector() {
		//delete[]data;
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
		return data[i - 1]; //ע���������������������������������������������������������������������������������������������������������������������������������
	}
};

template<class Type>
class BinaryTree {
private:
	struct Node {
		Node *left, *right;
		Type data;
		int itsDepth;
		int trueDepth=0;
		Node() :left(NULL), right(NULL) {}
		Node(Type item, Node *L = NULL, Node *R = NULL) :data(item), left(L), right(R) {}
		~Node() {}
	};
	Node *root;
	vector<Node *> treeNodes;
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
	void createTree(const Type &i, treevector<BinaryTree<int> > &trees) { //�ݹ飬���������Ҫ����Ķ��ӣ���ȥ��������
		BinaryTree lt, rt;
		if (sheet[i - 1][0] != nullFlag) {
			if (!createFlag[sheet[i - 1][0]]) {
				createTree(sheet[i - 1][0], trees);
				lt = trees.visit(sheet[i - 1][0]);
			}
		}
		if (sheet[i - 1][1] != nullFlag) {
			if (!createFlag[sheet[i - 1][1]]) {
				createTree(sheet[i - 1][1], trees);
				rt = trees.visit(sheet[i - 1][1]);
			}
		}
		if (!createFlag[i]) {
			makeTree(sheet[i - 1][2], lt, rt);
			trees.set(*this, i);
			createFlag[i] = 1;
		}
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
	int getTrueDepth() {
		return root->trueDepth;
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
	void setTrueDepth() {
		setTrueDepth(root,TrueHeight);
	}
	/*void setDepth(int heightnum) {
		setDepth(root,Height,heightnum);
	}*/
	void preOrder() {
		preOrder(root);
	}
	void postOrder() {
		postOrder(root);
	}
	void rowOrder() {
		int sizeOfTree = treeNodes.size();
		for (int i = 0; i < sizeOfTree;++i) {
			TrueHeight = (TrueHeight>treeNodes.visit(i)->trueDepth? TrueHeight: treeNodes.visit(i)->trueDepth);
		}
		for (int i = 1; i <= TrueHeight; ++i) {
			for (int j = 0; j < sizeOfTree; ++j) {
				if (treeNodes.visit(j)->trueDepth == i) {
					cout << treeNodes.visit(j)->data << ' ';
				}
			}
		}
	}
	void travel() {
		Node * p[100000];
		p[0] = root;

		int front = 0;
		int rear = 0;

		while (rear >= front) {

			cout << p[front]->data << ' ';

			if (p[front]->left != NULL) {
				p[rear + 1] = p[front]->left;
				rear++;
			}

			if (p[front]->right != NULL) {
				p[front] = p[front]->right;
				front--;
			}
			front++;
		}
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
	void setTrueDepth(Node *t,int height) {
		if (t!=NULL) {
			treeNodes.push(t);
			t->trueDepth = height;
			setTrueDepth(t->left, ++height);
			--height;
			setTrueDepth(t->right,height);
		}
	}
	/*void setDepth(Node *t, int &height, int &heightnum)const { //ǰ��������ø����ڵ�����,heightnum���ܸ߶ȣ�number�ǵ����ڶ��еĽ����Ŀ
		if (t != NULL) {
			height++; //�½�һ��
					  //cout << t->data << ' ';
					  //cout << Height << endl;
			t->itsDepth = height;
			setDepth(t->left, height, heightnum);
			setDepth(t->right, height, heightnum);
			height--; //����һ��
		}
	}*/
	void preOrder(Node *root) {
		if (root != NULL) {
			cout << root->data << ' ';
			preOrder(root->left);
			preOrder(root->right);
		}
	}
	void postOrder(Node *root) {
		if (root != NULL) {
			postOrder(root->left);
			cout << root->data << ' ';
			postOrder(root->right);
		}
	}
};


int main() {
	/*freopen("text.txt", "w", stdout);
	cout << 10 << endl;
	cout << 2 << ' ' << 0 << ' ' << 1<<endl;
	for (int i = 2; i < 200;++i) {
		cout << 0 << ' '<< i+1 << ' ' << i<<endl;
	}*/
	freopen("test2.txt", "r", stdin);
	int N;
	cin >> N;
	int leftChild, rightBrother, selfData;
	treevector<BinaryTree<int> > trees;
	int sizemax = 0, maxPosition = 0;
	BinaryTree<int> smallTree;
	BinaryTree<int> largeTree;
	trees.setLength(N);
	for (int i = 0; i < N; ++i) {
		cin >> leftChild >> rightBrother >> selfData;
		sheet[i][0] = leftChild;
		sheet[i][1] = rightBrother;
		sheet[i][2] = selfData;
	}
	for (int i = 1; i <= N; ++i) {
		smallTree.createTree(i, trees);
	}
	for (int i = 1; i <= N; ++i) {
		if (sizemax < trees.visit(i).size()) {
			sizemax = trees.visit(i).size();
			maxPosition = i;
		}
	}
	largeTree = trees.visit(maxPosition); //�����
	//largeTree.setTrueDepth();
	largeTree.preOrder();
	cout << endl;
	largeTree.postOrder();
	cout << endl;
	//largeTree.rowOrder();
	largeTree.travel();
	return 0;
}