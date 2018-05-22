#include<iostream>
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
int sizeOfStr(char *input) { //求取长度
	int i = 0;
	while (input[i] != '\0') {
		++i;
	}
	return i;
}
bool strEqual(char *a, char *b) {
	if (sizeOfStr(a) != sizeOfStr(b)) {
		return false;
	}
	int lengthOfStr = sizeOfStr(a);
	for (int i = 0; i < lengthOfStr; ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}
const int IF_ELSE = 0;
const int BEGIN_END = 1;

int main() {
	char p[1000] = { '\0' };
	//freopen("test.txt", "r", stdin);
	seqStack<int> left(100);
	int elseNum = 0;
	bool flag = false;
	while (cin >> p) {
		if (strEqual(p, "begin")) {
			left.push(BEGIN_END);
			continue;
		}
		if (strEqual(p, "end")) {
			if (left.isEmpty()) {
				flag = true;
				continue;
			}
			if (left.pop() != BEGIN_END) { flag = true; }
			continue;
		}
		if (strEqual(p, "if")) {
			left.push(IF_ELSE);
			continue;
		}
		if (strEqual(p, "then")) {
			if (left.pop() != IF_ELSE) { flag = true; }
			++elseNum;
			continue;
		}
		if (strEqual(p, "else")) {//else可有可无，但是如果有则必须仅有一个，且else一定是出现在then后的
			if (elseNum==0) {
				flag = true;
			}
			--elseNum;
			continue;
		}
	}
	if (flag || !left.isEmpty()) {
		cout << "Error!" << endl;
	}
	else {
		cout << "Match!" << endl;
	}
	return 0;
}