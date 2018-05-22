#include<iostream> //可能是递归太深导致堆栈溢出？
#include<cstring>
#pragma warning (disable:4996)
using namespace std;

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

template<class elemType>
class binarySearchTree {
private:
	struct treeNode {
		elemType data;
		treeNode *left;
		treeNode *right;
		treeNode(elemType t = 0,treeNode *lt=NULL,treeNode *rt=NULL) :data(t),left(lt),right(rt){}
	};
public:
	int getsize() {
		return getsize(root);
	}
	binarySearchTree(treeNode *t = NULL) { root = t; }
	~binarySearchTree(){}
	void insert(elemType input) {
		insert(input, root);
	}
	bool find(elemType input) {
		return find(input, root);
	}
	elemType findIth(int input) {
		int answer = 0;
		return findIth(input, root,answer);
	}
	void de(elemType input) {
		de(input, root);
	}
	void deLessThan(elemType input) {
		deLessThan(input, root);
	}
	void deGreaterThan(elemType input) {
		deGreaterThan(input, root);
	}
	void deInterval(elemType inputFirst,elemType inputSecond) {
		deInterval(inputFirst, inputSecond, root);
	}
	void makeEmpty(treeNode *&t) {
		if (t == NULL)return;
		else {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
	}
private:
	treeNode *root;
	void insert(elemType input,treeNode* &node) {
		if (node == NULL) {
			node = new treeNode(input,NULL,NULL);
		}
		else if (input<node->data) { //等于算在了大于里
			insert(input,node->left);
		}
		else {
			insert(input,node->right);
		}
	}
	bool find(elemType input, treeNode* node) {
		if (node == NULL) { return false; }
		else if (input<node->data) {
			return find(input,node->left);
		}
		else if (input>node->data) {
			return find(input,node->right);
		}
		else return true;
	}
	elemType findIth(int &input, treeNode* node,int answer) { //用中序遍历就行
		if (node->left != NULL) {
			answer = findIth(input, node->left,answer);
		}
		--input;    //不能加条件，确保第一次找到返回，其他不返回
		if (input == 0) { 
			return node->data;
		}
		if (node->right != NULL) {
			answer = findIth(input, node->right,answer);
		}
		return answer;
	}
	void de(elemType input, treeNode* &node) {
		if (node == NULL) { return; }
		else if (input<node->data) {
			de(input,node->left);
		}
		else if (input>node->data) {
			de(input, node->right);
		}
		else {
			if (node->left!=NULL&&node->right!=NULL) {
				treeNode *temp=node->right;
				while (temp->left!=NULL) {
					temp = temp->left;
				}
				node->data = temp->data; //取右儿子最小值或左儿子最大值去弥补，然后删掉弥补所用的结点，此处用的是右边的最小
				de(node->data,node->right);
			}
			else {
				treeNode *oldNode = node;
				node = node->left == NULL ? node->right : node->left;
				delete oldNode;
			}
		}
	}
	/*void deLessThan(elemType input, treeNode* &node) { //input是否永远在树中？想用二叉查找树的优点，但是这个写错了有问题
		if (node == NULL)return;
		if (node->data>=input) {  //不能用>，因为有等于的情况，不能删除
			deLessThan(input, node->left);
		}
		else { 
			node->left = NULL;
			if (node->right != NULL) {
				if (node->right->data<input) {
					node = NULL;
				}
				else { 
					node = node->right;
					deLessThan(input, node);
				}
			}
			else {
				node = NULL;
			}
		}
	}
	void deGreaterThan(elemType input, treeNode* &node) {  //
		if (node == NULL)return;
		if (node->data<=input) {
			deGreaterThan(input, node->right);
		}
		else {
			node->right = NULL;
			if (node->left != NULL) {
				if (node->left->data>input) {
					node = NULL;
				}
				else {
					node = node->left;
					deGreaterThan(input, node);
				}
			}
			else {
				node = NULL;
			}
		}
	}*/

	/*void deLessThan(elemType input, treeNode* &node) { //依次查找删除，没有利用二叉查找树的特性，但是对的
		if (node == NULL)return;
		if (node->data<input) {
			de(node->data, node);
			deLessThan(input, node); //要再次删除一下这个地方，看新补上来的数是否是符合情况的
		}
		if (node != NULL) { deLessThan(input, node->left); }
		if (node != NULL) { deLessThan(input, node->right); }
		return;
	}
	void deGreaterThan(elemType input, treeNode* &node) {
		if (node == NULL)return;
		if (node->data>input) {
			de(node->data, node);
			deGreaterThan(input, node); //要再次删除一下这个地方，看新补上来的数是否是符合情况的
		}
		if (node != NULL) { deGreaterThan(input, node->left); }
		if (node != NULL) { deGreaterThan(input, node->right); }
		return;
	}
	void deInterval(elemType inputFirst, elemType inputSecond, treeNode* &node) { //这里有问题
		if (node == NULL)return;
		if (node->data>inputFirst&&node->data<inputSecond) {
			de(node->data, node);
			deInterval(inputFirst, inputSecond, node); //要再次删除一下这个地方，看新补上来的数是否是符合情况的
		}
		if (node != NULL) { deInterval(inputFirst, inputSecond, node->left); }
		if (node != NULL) { deInterval(inputFirst, inputSecond, node->right); }
		return;
	}*/
	void deLessThan(elemType input, treeNode* &node) { //依次查找删除，没有利用二叉查找树的特性
		if (node == NULL)return;
		if (node->data < input) {
			treeNode *tmp = node;
			node = node->right;
			makeEmpty(tmp->left);
			delete tmp;
			deLessThan(input, node);
		}
		else {
			deLessThan(input, node->left);
		}
	}
	void deGreaterThan(elemType input, treeNode* &node) {
		if (node == NULL)return;
		if (node->data > input) {
			treeNode *tmp = node;
			node = node->left;
			makeEmpty(tmp->right);
			delete tmp;
			deGreaterThan(input, node);
		}
		else {
			deGreaterThan(input, node->right);
		}
	}
	void deInterval(elemType inputFirst, elemType inputSecond, treeNode* &node) { //这里有问题
		if (node == NULL)return;
		if (node->data<=inputFirst) deInterval(inputFirst, inputSecond, node->right);
		else if (node->data>=inputSecond) deInterval(inputFirst, inputSecond, node->left);
		else {
			de(node->data, node);
			deInterval(inputFirst, inputSecond, node);
		}
	}
	int getsize(treeNode* p) {
		if (p==NULL) { return 0; }
		else return getsize(p->left) + getsize(p->right) + 1; //最开始写错了
	}
};

int main() {
	freopen("test2.txt","r",stdin);
	binarySearchTree<int> tree;
	int n;
	char order[20];
	cin >> n;
	for (int i = 0; i < n;++i) {
		cin >> order;
		if (strcmp(order,"insert")==0) {
			int input;
			cin >> input;
			tree.insert(input);
		}
		else if (strcmp(order, "find") == 0) {
			int input;
			cin >> input;
			if (tree.find(input)) {
				cout << "Y" << endl;
			}
			else {
				cout << "N" << endl;
			}
		}
		else if (strcmp(order, "find_ith") == 0) {
			int input;
			cin >> input;
			if (input>tree.getsize()) {
				cout << "N" << endl;
			}
			else {
				cout<<tree.findIth(input)<<endl;
			}
		}
		else if (strcmp(order, "delete") == 0) {
			int input;
			cin >> input;
			tree.de(input);
		}
		else if (strcmp(order, "delete_less_than") == 0) {
			int input;
			cin >> input;
			tree.deLessThan(input);
		}
		else if (strcmp(order, "delete_greater_than") == 0) {
			int input;
			cin >> input;
			tree.deGreaterThan(input);
		}
		else if (strcmp(order, "delete_interval") == 0) {
			int inputFirst, inputSecond;
			cin >> inputFirst >> inputSecond;
			tree.deInterval(inputFirst, inputSecond);
		}
		//cout << "size is"<<tree.getsize()<<endl;
	}
	return 0;
}