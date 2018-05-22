#include<iostream>
#include<cstring>
#pragma warning (disable:4996)
const int INT = 2147483647;
using namespace std;
class priorityQueue {
public:
	priorityQueue(int capacity=100) {
		array = new int[capacity];
		maxSize = capacity;
		currentSize = 0;
	}
	~priorityQueue() {}
	bool isEmpty() {
		return currentSize == 0;
	}
	void enQueue(int x) {
		if (currentSize == maxSize - 1)doubleSpace();
		int hole = ++currentSize;
		for (; hole > 1 && x < array[hole / 2];hole/=2) {
			array[hole] = array[hole / 2];
		}
		array[hole] = x;
	}
	void find(int x) {
		int weight=INT, pos = 0;
		for (int i = 1; i <= currentSize;++i) {
			if (array[i]>x) {
				if (array[i] < weight) {
					weight = array[i];
					pos = i;
				}
				else if(array[i] == weight){
					if (i < pos) { pos = i; }
				}
			}
		}
		cout << pos << endl;
	}
	void decrease(int i,int v) {
		array[i] -= v;
		int hole = i,x=array[i];
		for (; hole > 1 && x < array[hole / 2];hole/=2) {
			array[hole] = array[hole / 2];
		}
		array[hole] = x;
	}
	void show() {
		for (int i = 1; i <= currentSize; ++i) {
			cout << array[i] << " ";
		}
		cout << endl;
	}
private:
	int currentSize;
	int *array;
	int maxSize;
	void doubleSpace() {
		int *temp = array;
		maxSize *= 2;
		array = new int[maxSize];
		for (int i = 0; i <= currentSize;++i) {
			array[i] = temp[i];
		}
		delete[]temp;
	}
	void percolateDown(int hole) {
		int child;
		int temp = array[hole];
		for (; hole * 2 <= currentSize; hole = child) {
			child = hole * 2;
			if (child!=currentSize&&array[child+1]<array[child]) {
				child++;
			}
			if (array[child]<temp) {
				array[hole]=array[child];
			}
			else break;
		}
		array[hole] = temp;
	}
};
int main() {
	freopen("test.txt","r",stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int num;
	char order[9];
	priorityQueue queue;
	cin >> num;
	for (int i = 0; i < num;++i) {
		cin >> order;
		if (strcmp(order,"insert")==0) {
			int x;
			cin >> x;
			queue.enQueue(x);
		}
		else if (strcmp(order,"find")==0) {
			int x;
			cin >> x;
			queue.find(x);
		}
		else {
			int i,v;
			cin >> i>>v;
			queue.decrease(i,v);
		}
	}
}