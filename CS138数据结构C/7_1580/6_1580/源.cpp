#include<iostream>
#include<vector>
#pragma warning (disable:4996)
using namespace std;
int input[1000001];
class dp {
public:
	int amount = 0;
	int last = 0;
	dp(int a,int b) :amount(a),last(b){}
    dp():amount(0),last(0){}
};
dp dpList[1000001];
int dpLast[1000001] = {0};
int binarySearch(int in,int start,int end) {
	if (start == end) {
		if (dpLast[start]>=in) { //这里是取得下界lower_bound，不加等于是上界upper_bound
			return start;
		}
		else {
			return start+1;
		}
	}
	int mid = (start+end) / 2;
	if (dpLast[mid]>=in) {   //同样这里是取得下界lower_bound，不加等于是上界upper_bound
		binarySearch(in, start, mid);
	}
	else {
		binarySearch(in, mid+1, end);
	}
}
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int n;
	cin >> n;
	for (int i = 0; i < n;++i) {
		cin >> input[i];
	}

	//n*lgn算法
	int length = 1;
	dpLast[0] = input[0];
	for (int i = 1; i < n;++i) {
		if (dpLast[length-1]<input[i]) {
			//cout << length << endl;
			dpLast[length++] = input[i];
		}
		else {
			int pos = binarySearch(input[i], 0, length-1); //这里最开始写成了i-1
			//cout << pos<<endl;
			dpLast[pos] = input[i];
		}
	}
	cout << length;
	
	/*dp first(1,input[0]);
	dpList[0]=first;
	int answer = 1;
	for (int i = 1; i < n;++i) { //i表示到了前i个数的处理
		int pos = -1;
		int posAmount = 0;
		for (int j = i - 1; j >= 0;--j) {
			if (input[i]>dpList[j].last&&dpList[j].amount>posAmount) {
				pos = j;
				posAmount = dpList[j].amount;
			}
		}
		if (pos>-1) {
			dp temp(posAmount + 1, input[i]);
			dpList[i]=temp;
            if(answer<posAmount+1){
                answer=posAmount+1;
            }
		}
		else {
			dp temp(1, input[i]);
			dpList[i]=temp;
		}
		
	}
	cout << answer; //n^2算法
	*/
	
	return 0;
}