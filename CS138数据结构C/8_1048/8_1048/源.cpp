#include<iostream>
#pragma warning (disable:4996)
using namespace std;
template<class elemType>
class linkQueue {
private:
	struct node {
		elemType data;
		node *next;
		node(const elemType &x, node *N = NULL) {
			data = x;
			next = N;
		}
		node() :next(NULL) {}
		~node() {}
	};
	node *front, *rear;
public:
	linkQueue() { front = rear = NULL; }
	~linkQueue();
	bool isEmpty() {
		return front == NULL;
	}
	void enQueue(const elemType &x);
	elemType deQueue() {
		node *tmp = front;
		elemType value = front->data;
		front = front->next;
		if (front == NULL) {
			rear = NULL;
		}
		delete tmp;
		return value;
	}
	elemType getHead() { return front->data; }
	void perMinute() {
		--front->data;
	}
};
template<class elemType>
linkQueue<elemType>::~linkQueue() {
	node *tmp;
	while (front != NULL) {
		tmp = front;
		front = front->next;
		delete tmp;
	}
}
template<class elemType>
void linkQueue<elemType>::enQueue(const elemType &x) {
	if (rear == NULL) {
		front = rear = new node(x);
	}
	else {
		rear->next = new node(x);
		rear = rear->next;
	}
}
class treeNode {
public:
	int data;
	treeNode *leftSon;
	treeNode *rightSon;
	treeNode *parent;
	treeNode():data(0),leftSon(NULL),rightSon(NULL){}
};
treeNode tree[1200];
linkQueue<treeNode> outputQueue;
void levelOrder(int root) {
	outputQueue.enQueue(tree[root]);
	while (!outputQueue.isEmpty()) {
		treeNode tmp = outputQueue.deQueue();
		cout << tmp.data << endl;
		if (tmp.leftSon!=NULL) {
			outputQueue.enQueue(*tmp.leftSon);
		}
		if (tmp.rightSon != NULL) {
			outputQueue.enQueue(*tmp.rightSon);
		}
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	freopen("test.txt","r",stdin);
	int n,root;
	cin >> n;
	int self, leftson, rightson;
	for (int i = 1; i <= (n - 1) / 2;++i) {
		cin >> self >> leftson >> rightson;
		tree[self].data = self;
		tree[leftson].data = leftson;
		tree[rightson].data = rightson;
		tree[self].leftSon = &tree[leftson];
		tree[self].rightSon = &tree[rightson];
		tree[leftson].parent = &tree[self];
		tree[rightson].parent = &tree[self];
	}
	for (int i = 1; i <= n;++i) { //这里最开始写错了写成了i<=(n-1)/2，注意想略去最后一层应该是保存self然后i<=(n-1)/2用self做下标，原来下标写错了
		if (tree[i].parent == NULL) { root = i; }
	}
	levelOrder(root);
	return 0;
}