#include<iostream>
using namespace std;
int weight[10005] = { 0 }; //优先队列方法
int answer[10005] = { 0 };
class priorityQueue {
public:
	priorityQueue(int capacity = 100) {
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
		for (; hole > 1 && x < array[hole / 2]; hole /= 2) {
			array[hole] = array[hole / 2];
		}
		array[hole] = x;
	}
	int deQueue() {
		int minItem;
		minItem = array[1];
		array[1] = array[currentSize--];
		percolateDown(1);
		return minItem;
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
		for (int i = 0; i <= currentSize; ++i) {
			array[i] = temp[i];
		}
		delete[]temp;
	}
	void percolateDown(int hole) {
		int child;
		int temp = array[hole];
		for (; hole * 2 <= currentSize; hole = child) {
			child = hole * 2;
			if (child != currentSize&&array[child + 1]<array[child]) {
				child++;
			}
			if (array[child]<temp) {
				array[hole] = array[child];
			}
			else break;
		}
		array[hole] = temp;
	}
};
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	priorityQueue que;
	int num;
	cin >> num;
	for (int i = 0; i < num; i++) {
		cin >> weight[i];
		que.enQueue(weight[i]);
	}
	int i = 0;
	while (!que.isEmpty()) {
		int first=0,second=0;
		first = que.deQueue();
		if (que.isEmpty()) break;
		second = que.deQueue();
		answer[i] = first + second;
		que.enQueue(answer[i]);
		++i;
	}
	int answers = 0;
	for (int j = 0; j < i;++j) {
		answers += answer[j];
	}
	cout << answers;
}
/*#include<iostream> //哈夫曼树方法
using namespace std;
int weight[10005] = {0};
class huffmanTree {
private:
	struct node {
		int weightNum;
		int parent, left, right;
		node() :parent(0),left(0),right(0),weightNum(0){}
	};
	node* elem;
	int length;
public:
	huffmanTree(int size) {
		int min1, min2;//最小树、次最小树的权值
		int x, y;//次最小树、最小树的下标

		length = size * 2;
		elem = new node[length];

		for (int i = size; i < length;++i) {
			elem[i].weightNum = weight[i - size];
		}
		const int maxWeight = 20000;
		for (int i = size - 1; i>0;--i) {
			min1 = min2 = maxWeight+1;
			x = y = 0;
			for (int j = i + 1; j < length;++j) {
				if (elem[j].parent==0) {
					if (elem[j].weightNum<min1) {
						min2 = min1; 
						min1 = elem[j].weightNum;
						x = y; 
						y = j; 
					}
					else if (elem[j].weightNum<min2) {
						min2 = elem[j].weightNum;
						x = j;
					}
				}
			}
			elem[i].weightNum = min1 + min2;
			elem[i].left = x;
			elem[i].right = y;
			elem[i].parent = 0;
			elem[x].parent = i;
			elem[y].parent = i;
		}
	}
	~huffmanTree(){}
	void totalSum(int &answer) {
		for (int i = 1; i < length/2;++i) {
			answer += elem[i].weightNum;
		}
	}
};
int main() {
	int answer = 0;
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int num;
	cin >> num;
	for (int i = 0; i < num; i++) {
		cin >> weight[i];
	}
	huffmanTree tree(num);
	tree.totalSum(answer);
	cout << answer;
	return 0;
}*/