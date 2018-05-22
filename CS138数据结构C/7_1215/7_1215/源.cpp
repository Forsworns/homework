#include<iostream>
#include<cstring>
#pragma warning (disable:4996)
using namespace std;
template<class Type>
class priorityQueue {
public:
	priorityQueue(int capacity=20000) {
		array = new Type[capacity];
		maxSize = capacity;
		currentSize = 0;
	}
	~priorityQueue() { /*delete[]array;*/ }

	void enQueue(const Type &x);
	Type deQueue();
	Type getHead()const { 
		return array[1]; 
		/*cout << endl;
		for (int j = 1; j <= currentSize; ++j) {
			cout << array[j] << " ";
		}
		cout << endl;*/
	}
private:
	int currentSize;
	Type *array;
	int maxSize;
	void doubleSpace();
	void percolateDown(int hole);
};
template<class Type>
void priorityQueue<Type>::doubleSpace() {
	Type *tmp = array;
	maxSize *= 2;
	array = new Type[maxSize];
	for (int i = 0; i <= currentSize;++i) {
		array[i] = tmp[i];
	}
	delete[]tmp;
}
template<class Type>
void priorityQueue<Type>::percolateDown(int hole) {
	int child;
	Type tmp = array[hole];
	for (; hole * 2 <= currentSize;hole=child) {
		child = hole * 2;
		if (child != currentSize&&array[child + 1] < array[child])child++;
		if (array[child] < tmp)array[hole] = array[child];
		else break;
	}
	array[hole] = tmp;
}
template<class Type>
void priorityQueue<Type>::enQueue(const Type &x) {
	if (currentSize==maxSize-1) {
		doubleSpace();
	}
	int hole = ++currentSize;
	for (; hole > 1 && x < array[hole / 2];hole/=2) {
		array[hole] = array[hole / 2];
	}
	array[hole] = x;
	/*cout << endl;
	for (int j = 1; j <= currentSize; ++j) {
		cout << array[j] << " ";
	}
	cout << endl;*/
}
template<class Type>
Type priorityQueue<Type>::deQueue() {
	Type minItem;
	minItem = array[1];
	array[1] = array[currentSize--];
	percolateDown(1);
	/*cout << endl;
	for (int j = 1; j <= currentSize; ++j) {
		cout << array[j] << " ";
	}
	cout << endl;*/
	return minItem;
}
int main() {
	freopen("test.txt","r",stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int orderNum,insertNum;
	char order[7];
	cin >> orderNum;
	priorityQueue<int> input;
	for (int i = 0; i < orderNum;++i) {
		cin >> order;
		if (strcmp(order,"insert")==0) {
			cin >> insertNum;
			input.enQueue(insertNum);
		}
		else if (strcmp(order,"delete")==0) {
			input.deQueue();
		}
		else {
			cout << input.getHead() << endl;
		}
	}
	return 0;
}