#pragma warning(disable : 4996)
#include<iostream>
#include<stdio.h>
using namespace std; //��Ӧ����˫������ģ���ʱ
class doubleLinkList {
private:
	struct node {
		int data;
		node *next;
		node():next(NULL){}
		node(const int &elem,node *n = NULL) {
			data = elem;
			next = n;
		}
		~node(){}
	};
	int currentLength;
	node *head, *tail;
	node *move(int i)const;
public:
	doubleLinkList();
	void insertAtHead(const char &x);
	int visit(int i)const;
	~doubleLinkList(){}
	doubleLinkList operator+(const doubleLinkList &another); //���ַ�����ĺ�����������ͻ��ô�������
    doubleLinkList operator=(const doubleLinkList &another);
	friend ostream & operator<<(ostream &os, const doubleLinkList&);
};
ostream & operator<<(ostream &os, const doubleLinkList& answer) {
	int length = answer.currentLength-1;
	int *answerNum = new int[answer.currentLength];
	doubleLinkList::node *tmpNode = answer.head->next;
	for (int j = length; j >= 0; --j) {
		answerNum[j]= tmpNode->data;
		tmpNode = tmpNode->next;
	}
	for (int j = 0; j <answer.currentLength; ++j) {//��󻹳�ʱ����Ϊ�������������ڲ���move(i)�ˣ�ֱ�Ӵ洢��������O(n)������ᵽO(n*n)���������������
		printf("%d", answerNum[j]);
	}
	return os;
}
doubleLinkList::node *doubleLinkList::move(int i)const { //ע��i--�Ƿ��ز���ǰ��ֵ
	node *p = head->next;
	while (i--) p = p->next;
	return p;
}
int doubleLinkList::visit(int i)const {
	node *p = move(i);
	return p->data;
}
doubleLinkList::doubleLinkList() {
	head = new node;
	head->next = tail = new node;
	currentLength = 0;
}

void doubleLinkList::insertAtHead(const char &x) {
	head->next = new node(x - '0', head->next);
	++currentLength;
}

doubleLinkList doubleLinkList::operator+(const doubleLinkList &another) {
	doubleLinkList tmp;
	node *tmpNodeAnswer, *tmpNodeThis, *tmpNodeAnother;
	tmpNodeAnswer = tmp.head->next = new node;
	tmpNodeThis = head->next;
	tmpNodeAnother = another.head->next;
	int flagAddUpper = 0; //�жϽ�λ

	while (tmpNodeThis != tail && tmpNodeAnother != another.tail ) {
		int tempData = tmpNodeThis->data + tmpNodeAnother->data + flagAddUpper;
		tmpNodeAnswer->data = (tempData) % 10 ; //���
		flagAddUpper = (tempData) / 10; //���ý�λflag

		tmpNodeAnswer = tmpNodeAnswer->next = new node;	   //�л��ڵ�
		tmpNodeThis = tmpNodeThis->next;
		tmpNodeAnother = tmpNodeAnother->next;
	}

	if (tmpNodeThis==tail) {
		tmp.currentLength = another.currentLength;
		while (tmpNodeAnother != another.tail) {
			int tempData = tmpNodeAnother->data + flagAddUpper;
			tmpNodeAnswer->data = (tempData) % 10;
			flagAddUpper = (tempData) / 10;

			tmpNodeAnswer = tmpNodeAnswer->next = new node;
			tmpNodeAnother = tmpNodeAnother->next;
		}
		if (flagAddUpper) { //������н�λ����ô����½ڵ�����Ҫ�ģ�����ֱ�ӽ��ɽڵ����ӵ�β������
			tmpNodeAnswer->data = flagAddUpper;
			tmpNodeAnswer->next = tmp.tail;
			tmp.currentLength++;
		}
		else {
			tmpNodeAnswer = tmp.tail;
			//delete tmpNodeAnswer;
		}
	}
	else {
		tmp.currentLength = currentLength;
		while (tmpNodeThis != tail) {
			int tempData = tmpNodeThis->data + flagAddUpper;
            tmpNodeAnswer->data = (tempData) % 10;
			flagAddUpper = (tempData) / 10;

			tmpNodeAnswer = tmpNodeAnswer->next = new node;
			tmpNodeThis = tmpNodeThis->next;
		}
        if (flagAddUpper) {
			tmpNodeAnswer->data = flagAddUpper;
			tmpNodeAnswer->next = tmp.tail;
			tmp.currentLength++;
		}
		else {
			tmpNodeAnswer = tmp.tail;
			//delete tmpNodeAnswer;
		}
	}
	return tmp;
}
doubleLinkList doubleLinkList::operator=(const doubleLinkList &another){
	if (this == &another) {
		return *this;
	}
	currentLength = another.currentLength;
	node *tmpAnother = another.head->next;
	node *tmpThis = head;
	for (int i = 0; i < currentLength;++i) {
		tmpThis->next = new node;
		tmpThis->next->data = tmpAnother->data;
		tmpThis = tmpThis->next;
		tmpAnother = tmpAnother->next;
	}
	tmpThis->next = tail;
	return *this;
}
int main() {
	doubleLinkList first;
	doubleLinkList second;
	char everyNum[1000000];
	int i = 0;
	scanf("%s",everyNum);
	while (everyNum[i]!='\0') {
		first.insertAtHead(everyNum[i]);
		++i;
	}
	i = 0;
	scanf("%s", everyNum);
	while (everyNum[i] != '\0') {
		second.insertAtHead(everyNum[i]);
		++i;
	}

	doubleLinkList answer;
	answer = first + second;
	cout << answer << endl;
	return 0;
}