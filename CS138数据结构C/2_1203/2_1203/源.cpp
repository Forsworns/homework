#include<iostream>
using namespace std;

template <class T>
class list {//使用模板类
public:
	T *data; //这里最好不要放到public里，但是为了输入方便先放这里了
	int length;
	list(int num) {
		length = num;
		data = new T[length];
	}
	~list() { //这里析构函数怎么加？

	}
	list operator+(const list &another) {
		list tmp(length + another.length);
		for (int i = 0; i < length; ++i) {
			tmp.data[i] = data[i];
		}
		for (int j = 0; j < another.length; ++j) {
			tmp.data[length + j] = another.data[j];
		}
		return tmp;
	}
	list operator=(const list &another) {
		if (this == &another) return *this;
		length = another.length;
		data = new T[length];
		for (int i = 0; i < length;++i) {
			data[i] = another.data[i];
		}
		return *this;
	}
};

int strlength(const char* strings) {
	int count = 0;
	while(strings[count]!='\0') {
		++count;
	}
	return count;
}

int main() {
	char typeName[8];
	int n = 0, m = 0;
	cin >> typeName;
	cin >> n >> m;
	switch (strlength(typeName)) {//由输入的长度判断
		case 3: {
			list<int> input1(n);
			list<int> input2(m);
			list<int> output(m+n);
			for (int i = 0; i < n; ++i) {
				cin >> input1.data[i];
			}
			for (int j = 0; j < m; ++j) {
				cin >> input2.data[j];
			}
			output = input1 + input2;
			for (int k = 0; k < output.length - 1; ++k) {
				cout << output.data[k] << ' ';
			}
			cout << output.data[output.length - 1];
			break;
		}
		case 4: {
			list<char> input1(n);
			list<char> input2(m);
			list<char> output(m + n);
			for (int i = 0; i < n; ++i) {
				cin >> input1.data[i];
			}
			for (int j = 0; j < m; ++j) {
				cin >> input2.data[j];
			}
			output = input1 + input2;
			for (int k = 0; k < output.length - 1; ++k) {
				cout << output.data[k] << ' ';
			}
			cout << output.data[output.length - 1];
			break;
		}
		case 6: {
			list<double> input1(n);
			list<double> input2(m);
			list<double> output(m + n);
			for (int i = 0; i < n; ++i) {
				cin >> input1.data[i];
			}
			for (int j = 0; j < m; ++j) {
				cin >> input2.data[j];
			}
			output = input1 + input2;
			for (int k = 0; k < output.length - 1; ++k) {
				cout << output.data[k] << ' ';
			}
			cout << output.data[output.length - 1];
			break;
		}
	}
	return 0;
}