#include<iostream>
#pragma warning (disable:4996)
using namespace std;
int arrayWaitSort[1000000];
int sortTime = 0;
int heap[1000000];
void percolateDown(int hole,int size) {
	int child;
	int tmp = arrayWaitSort[hole];
	
}
void push(int item,int pos) {

}
void pop(int &size) {
	int tmp = heap[0];
	heap[0] = heap[size-1];
	//percolateDown();
	heap[--size] = tmp;//删除后倒序存储下来，没有真的删除掉
}
void heapSort(int size) {
	for (int i = 0; i < size;++i) {
		push(arrayWaitSort[i],i);
	}
	for (int i = size; i > 0;--i) {
		pop(size);
	}
}
void mergeSort(int start,int end) {
	if (start>=end) {
		return;
	}
	int mid = (start + end-1) / 2;
	mergeSort(start,mid);
	mergeSort(mid + 1, end);
	int *tmp = new int[end-start+1];
	int left = start, right = mid+1, pos=0;//pos最开始写错了，写成了start但是因为这里的tmp的大小刚好为正在排列数组长度，所以那样会越界
	for (; left <= mid && right<=end;) {
		if (arrayWaitSort[left]<arrayWaitSort[right]) {
			tmp[pos] = arrayWaitSort[left];
			++left;
			++pos;
			++sortTime;
		}
		else {
			tmp[pos] = arrayWaitSort[right];
			++right;
			++pos;
			++sortTime;
		}
	}
	for (; left <= mid;++pos,++left) {
		tmp[pos] = arrayWaitSort[left];
	}
	for (; right <= end;++pos,++right) {
		tmp[pos] = arrayWaitSort[right];
	}
	for (int i = start; i <= end;++i) {
		arrayWaitSort[i] = tmp[i-start];
	}
	delete[]tmp;
}
int divide(int low,int high) {
	int tmp = arrayWaitSort[low];
	while (low<high) {
		while (low<high && tmp<arrayWaitSort[high]) {
			--high;
			++sortTime;
		}
		if (low < high) { arrayWaitSort[low++] = arrayWaitSort[high]; }
		while (low<high && tmp >= arrayWaitSort[low]) {
			++low;
			++sortTime;
		}
		if (low<high) { arrayWaitSort[high--] = arrayWaitSort[low]; }
	}
	arrayWaitSort[low] = tmp;
	return low;
}
void quickSort(int low,int high) {
	if (low>=high) {
		return;
	}
	int mid = divide(low,high);
	quickSort(low, mid);
	quickSort(mid+1, high);
}
int main() {//归并正确
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int size, type;
	cin >> size >> type;
	for (int i = 0; i < size;++i) {
		cin >> arrayWaitSort[i];
	}
	switch (type) {
	case 1:heapSort(size); break;
	case 2:mergeSort(0,size-1); break;
	case 3:quickSort(0,size-1); break;
	}
	/*for (int i = 0; i < size;++i) {
		cout << arrayWaitSort[i]<<" ";
	}cout << endl;*/
	cout << sortTime;
	return 0;
}