#include<iostream>
#include<string>
#include<algorithm>
#include<stdio.h>
#pragma warning (disable:4996)
using namespace std;
class List {
private:
	struct node {
		int val;
		node* next;
		node(int v, node* n = 0) :val(v), next(n) {}
	};
	node* head;
	int _size;
public:
	List();
	List(const List& other);
	List(const string & num);
	void clear();
	void push_front(int val);   // 在头部插入数值
	List operator+(const List& other);    //在这里进行链表加法实现
	List& operator=(const List& other);   // 赋值操作重载
	int size() const;
	~List();
	friend ostream& operator<<(ostream & os, const List &  out);
	// 输出数字，无需换行
};

List::List() {
	head = 0;
	_size = 0;
}

List::List(const List& other) {
	int* num = new int[other._size];
	node* current = other.head;
	int i = other._size - 1;
	while (current != 0) {
		num[i] = current->val;
		i--;
		current = current->next;
	}
	head = 0;
	_size = 0;
	for (int i = 0; i < other._size; i++)
		push_front(num[i]);
	delete[]num;
}

List::List(const string& num) {
	head = NULL;
	_size = 0;
	for (int i = 0; i < num.size(); i++)
		push_front(num[i] - 48);
}

void List::clear() {
	while (head != 0) {
		node* temp = head;
		head = head->next;
		delete temp;
	}
	_size = 0;
}

void List::push_front(int val) {
	node* temp = new node(val, head);
	head = temp;
	_size++;
}

List List::operator+(const List& other) {
	int max = _size > other._size ? _size : other._size;
	char* addNum = new char[max + 1];
	int carry = 0;
	List result;
	if (_size > other._size) {
		node* c1 = head, *c2 = other.head;
		int i = 0;
		while (c2 != 0) {
			addNum[i++] = (c1->val + c2->val + carry) % 10 + 48;
			carry = (c1->val + c2->val + carry) / 10;
			c1 = c1->next;
			c2 = c2->next;
		}
		while (c1 != 0) {
			addNum[i++] = (c1->val + carry) % 10 + 48;
			carry = (c1->val + carry) / 10;
			c1 = c1->next;
		}
		addNum[i] = carry + 48;
	}
	else {
		node* c1 = other.head, *c2 = head;
		int i = 0;
		while (c2 != 0) {
			addNum[i++] = (c1->val + c2->val + carry) % 10 + 48;
			carry = (c1->val + c2->val + carry) / 10;
			c1 = c1->next;
			c2 = c2->next;
		}
		while (c1 != 0) {
			addNum[i++] = (c1->val + carry) % 10 + 48;
			carry = (c1->val + carry) / 10;
			c1 = c1->next;
		}
		addNum[i] = carry + 48;
	}
	if (addNum[max] == '1') {
		for (int i = max; i >= 0; i--)
			result.push_front(addNum[i] - 48);
	}
	else {
		for (int i = max - 1; i >= 0; i--)
			result.push_front(addNum[i] - 48);
	}
	delete[]addNum;
	return result;
}

List& List::operator=(const List& other) {
	node* temp = head;
	int* num = new int[other._size];
	node* current = other.head;
	int i = other._size - 1;
	while (current != 0) {
		num[i] = current->val;
		i--;
		current = current->next;
	}
	head = NULL;
	_size = 0;
	for (int i = 0; i < other._size; i++)
		push_front(num[i]);
	while (temp != 0) {
		node* t = temp;
		temp = temp->next;
		delete t;
	}
	delete[]num;
	return *this;
}

int List::size() const { return _size; }

List::~List() {
	clear();
}

ostream& operator<<(ostream& os, const List& out) {
	char* num = new char[out._size + 1];
	List::node* current = out.head;
	int i = out._size - 1;
	num[out._size] = '\0';
	while (current != 0) {
		num[i] = current->val + 48;
		i--;
		current = current->next;
	}
	os << num;
	delete[]num;
	return os;
}

List methods[10002][2];
int main() {//0翘课，1不翘课，每天上课也算一种方式
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int daysNum;
	cin >> daysNum;
	methods[0][0].push_front(1);
	methods[0][1].push_front(1);
	methods[1][0].push_front(2);
	methods[1][1].push_front(2);
	for (int i = 2; i < daysNum;++i) {
		methods[i][1] = methods[i-1][0]+methods[i-1][1];
		methods[i][0] = methods[i-2][1]+methods[i-1][1];//你把这两行加起来其实按照递推关系就是前三项的和
	}
	cout << methods[daysNum-1][0] + methods[daysNum-1][1] << endl;
	return 0;
}