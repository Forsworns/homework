#include<iostream>
#include<algorithm>	//��ҮӦ����long long int û�й��ƺ÷�Χ
#pragma warning (disable:4996)
using namespace std;
int arrayInput[200000] = { 0 };
int tmp[200000] = { 0 };
long long int getAmount(int start,int end,int x,int arrayInput[]) {  //�������������У�ͳ��С�ڸ���������
	int pos;
	int tmpStart = start;
	while (start<end) {
		int mid = (start+end) / 2;
		if (2*arrayInput[mid]<x) {
			start = mid + 1;
			pos = start;
		}
		else {
			end = mid;
			pos = mid;
		}
	}
	
	return pos-tmpStart;
}
long long int mergeArrayAdd(int input[],int firstPos,int mid,int lastPos,int tmpSort[]) {
	long long int count=0;
	int frontStart = firstPos;
	int frontEnd = mid;
	int backStart = mid + 1;
	int backEnd = lastPos;
	int tmpSortPos = lastPos;
	while (frontStart<=frontEnd&&backStart<=backEnd) { //�ȹ鲢������������������ǲ�����Ҫ����
		if (input[frontEnd]<=input[backEnd]) {
			tmpSort[tmpSortPos--] = input[backEnd--];
		}
		else { //�ҵ����������ж��ǲ�����Ҫ��������ǣ������Ѿ��ų���˳����ô��������ĸ�Ԫ��ǰ��Ķ���
			count = count + getAmount(backStart, backEnd + 1, input[frontEnd],input); //����findPos�ǿ������൱��upper_bound
			/*cout <<"���" <<backStart << "�յ�"<<backEnd + 1 << "�Ƚ�ֵ"<<input[frontEnd];
			for (int i = backStart; i <= backEnd; ++i) {
				cout << " " << input[i] << " ";
			}
			cout << "���"<<getAmount(backStart, backEnd + 1, input[frontEnd],input) << endl;*/
			tmpSort[tmpSortPos--] = input[frontEnd--];  //ԭ����������Ҹ����ų�˳�򣬲���Ӱ��������Ե���Ҫ�����ϵ��֮ǰ�븴���ˣ����Ҳ��ų�˳�򲻺���
			
		}
	}
	while (frontStart<=frontEnd) {
		tmpSort[tmpSortPos--] = input[frontEnd--];
	}
	while(backStart<=backEnd) {
		tmpSort[tmpSortPos--] = input[backEnd--];
	}
	for (int i = firstPos; i <= lastPos;++i) {
		input[i] = tmpSort[i];
	}
	return count;
}
long long int  mergeSort(int input[],int firstPos,int lastPos,int tmpSort[]) {
	if (firstPos>=lastPos) {
		return 0;
	}
	int mid = (firstPos+lastPos) / 2;
	long long int left = mergeSort(input,firstPos,mid,tmpSort);
	long long int right = mergeSort(input,mid+1,lastPos,tmpSort);
	long long int all = mergeArrayAdd(input,firstPos,mid,lastPos,tmpSort);
	return (left + right + all);
}
int main() {
	freopen("test.txt","r",stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int n;
	cin >> n;
	for (int i = 0; i < n;++i) {
		cin >> arrayInput[i];
	}
	/*for (int i = 0; i < n; ++i) {
		cout<< arrayInput[i];
	}*/
	cout<<mergeSort(arrayInput, 0,n-1,tmp);
	return 0;
}