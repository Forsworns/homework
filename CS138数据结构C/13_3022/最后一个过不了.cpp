#include<iostream>
#include<string>
#include<algorithm>
#pragma warning (disable:4996)
using namespace std;
class bigInt {
	friend ostream &operator<<(ostream &os,const bigInt output);
public:
	int size = 0;
	int content[600] = { 0 };
	bigInt(){}
	bigInt(string input) {
		size = input.length();
		for (int i = 0; i < size;++i) {
			content[size-i-1] = input[i] - '0';
		}
	}
	bigInt operator+(bigInt &another) {
		int carry = 0;
		bigInt *temp=new bigInt;
		temp->size = max(size, another.size);
		for (int i = 0; i < temp->size;++i) {
			temp->content[i] = content[i] + another.content[i]+carry;
			carry = 0;
			if (temp->content[i] >= 10) {
				temp->content[i] = temp->content[i] - 10;
				carry = 1;
			}
		}
		if (carry) {
			temp->content[size] = temp->content[size] + 1;
			temp->size++;
		}
		return *temp;
	}
};
ostream &operator<<(ostream &os, const bigInt output) {
	for (int i = output.size - 1; i >= 0; --i) {
		os << output.content[i];
	}
	return os;
}
bigInt methods[10002][2];
int main() {//0翘课，1不翘课，每天上课也算一种方式
	/*string test1 = "9";
	string test2 = "12";
	bigInt a = test1;
	bigInt b = test2;
	cout << a << endl << b << endl << a + b;*/
	//freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int daysNum;
	cin >> daysNum;
	methods[0][0] = string("1");
	methods[0][1] = string("1");
	methods[1][0] = string("2");
	methods[1][1] = string("2");
	for (int i = 2; i < daysNum;++i) {
		methods[i][1] = methods[i-1][0]+methods[i-1][1];
		methods[i][0] = methods[i-2][1]+methods[i-1][1];
	}
	cout << methods[daysNum-1][0] + methods[daysNum-1][1] << endl;
	return 0;
}