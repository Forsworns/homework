#include <iostream>
#include <cstring>
using namespace std;


class T
{
	friend istream &operator>>(istream &in, T &obj)                          //输入 链表一个结点储存一行
	{
		node *tmp = obj.head, *p;
		for (int i = 0; i<100; i++) {
			p = tmp;
			tmp = new node;

			in.getline(tmp->data, 2000);
			obj.line++;
			p->next = tmp;
			if (strcmp(tmp->data, "******") == 0) { obj.line--; break; }
		}
		return in;

	};

private:
	struct node {                                //单链表
		char data[2002];
		node *next;

		node() {}

		~node() {};
	};
	node *head;                           //头指针
	int line;


public:
	T()
	{
		line = 0;
		head = new node();//构造函数
	}
	~T() {}




	void list(int num1, int num2)
	{
		if (num1 <= 0 || num2 <= 0 || num1>line || num2>line || num1>num2) { cout << "Error!" << endl; }
		else {
			node *tmp;
			tmp = head->next;
			for (int i = 0; i<line; i++)
			{
				if (i>num1 - 2 && i<num2) { cout << tmp->data << endl; }
				if (i == num2 - 1) { break; }
				tmp = tmp->next;
			}
		}
	}

	void ins(int num1, int num2, char *str)
	{
		node *tmp;
		tmp = head;
		if (num1 >= 1 && num1 <= line) { for (int i = 0; i < num1; i++) tmp = tmp->next; }  //指针移动到该行
		if (num1<1 || num2<1 || num1>line || num2>(strlen(tmp->data) + 1)) cout << "Error!" << endl;
		else {
			int R = strlen(tmp->data);
			for (int i = (strlen(tmp->data) - 1); i >= (num2 - 1); i--)
			{
				tmp->data[i + strlen(str)] = tmp->data[i];
			}
			tmp->data[R + strlen(str)] = '\0';
			for (int i = 0; i < strlen(str); i++)
			{
				tmp->data[i + num2 - 1] = str[i];
			}
		}
	}



	void del(int num1, int num2, int num3)
	{
		node * tmp;
		tmp = head;
		int R = 0;
		if (num1 >= 1 && num1 <= line) { for (int i = 0; i < num1; i++) { tmp = tmp->next; } R = strlen(tmp->data); }//指针指向行
		if (num1 < 1 || num2 < 1 || num1 > line || (num2 + num3) >(R + 1))
		{
			cout << "Error!" << endl;
		}
		else
		{
			for (int i = 0; i < (R - num3 - num2 + 1); i++)   //移动‘|0’
			{
				tmp->data[i + num2 - 1] = tmp->data[i + num3 + num2 - 1];
			}
			tmp->data[R - num3] = '\0';
		}

	}

	void quit()
	{
		node *tmp;
		tmp = head;
		for (int i = 0; i<line; i++) { tmp = tmp->next; cout << tmp->data << endl; }
	}

};

int main()
{
	T file;
	cin >> file;
	char w[5];
	char str[100];
	int num1, num2, num3;

	while (1)
	{
		cin >> w;
		if (strcmp(w, "list") == 0) {
			cin >> num1 >> num2;
			file.list(num1, num2);
		}
		if (strcmp(w, "ins") == 0)
		{
			cin >> num1 >> num2;
			cin.get();
			cin.getline(str, 100);
			file.ins(num1, num2, str);
		}
		else if (strcmp(w, "del") == 0)
		{
			cin >> num1 >> num2 >> num3;
			file.del(num1, num2, num3);
		}
		else if (strcmp(w, "quit") == 0) { break; }
	}
	file.quit();

	return 0;
}