#pragma warning(disable:4996) //如果首先给出的结点中的子结点尚未定义怎么破，就是不是叶节点在先？
#include<iostream>
#include<queue>
#include<cmath>
using namespace std;
class outOfBound {};
class illegalSize {};
const int nullFlag = 0;
int sheet[100000][3] = { 0 };
int createFlag[100000] = { 0 };
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
		int itsDepth;
		Node() :left(NULL), right(NULL) {}
		Node(Type item, Node *L = NULL, Node *R = NULL) :data(item), left(L), right(R) {}
		~Node() {}
	};
	queue<Node*> nodes;
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
			if (!createFlag[sheet[i - 1][0]-1]) {
				createTree(sheet[i - 1][0], trees);
			}
			lt = trees.visit(sheet[i - 1][0]);
		}
		if (sheet[i - 1][1] != nullFlag) {
			if (!createFlag[sheet[i - 1][1]-1]) {
				createTree(sheet[i - 1][1], trees);
			}
			rt = trees.visit(sheet[i - 1][1]);
		}
		if (!createFlag[i-1]) {
			makeTree(sheet[i-1][2], lt, rt);
			trees.set(*this, i);
			createFlag[i-1] = 1;
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
	void levelOrder() {
		levelOrder(root);
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
	void levelOrder(Node *t) {
		if (t!=NULL) {
			cout << t->data << ' ';
			if (t->left!=NULL) {
				nodes.push(t->left);
			}
			if (t->right!=NULL) {
				nodes.push(t->right);
			}
			if (!nodes.empty()) {
				t = nodes.front();
				nodes.pop();
				levelOrder(t);
			}
		}
	}
};

int main() {
	freopen("text.txt", "r", stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int N, leftSonData, rightSonData,selfData;
	treevector<BinaryTree<int> > trees;
	int sizemax = 0, maxPosition = 0;
	BinaryTree<int> smallTree;
	BinaryTree<int> largeTree;
	cin >> N;
	trees.setLength(N);
	for (int i = 0; i < N; ++i) {
		cin >> leftSonData >> rightSonData>>selfData;
		sheet[i][0] = leftSonData;
		sheet[i][1] = rightSonData;
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
	//cout << maxPosition << endl;
	//cout << trees.visit(maxPosition).size() << endl;
	//cout << trees.visit(maxPosition).height() << endl;
	//cout << trees.visit(maxPosition).getRoot() << endl;
	largeTree = trees.visit(maxPosition);
	largeTree.levelOrder();
	return 0;
}
