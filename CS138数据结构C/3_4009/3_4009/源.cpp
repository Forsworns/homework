#pragma warning(disable:4996)
#include<iostream>
using namespace std;

int sizeOfStr(char *input) { //求取长度
	int i = 0;
	while (input[i]!='\0') {
		++i;
	}
	return i;
}
bool strEqual(char *a,char *b) {
	if (sizeOfStr(a)!=sizeOfStr(b)) {
		return false;
	}
	int lengthOfStr = sizeOfStr(a);
	for (int i = 0; i < lengthOfStr;++i) {
		if (a[i]!=b[i]) {
			return false;
		}
	}
	return true;
}
int main() {
	char *input;
	char *secret;
	input = new char[100];
	secret = new char[100];
	//freopen("test.txt", "r",stdin);
	cin >> input;
	cin >> secret;
	int length = sizeOfStr(input);
	char **output;
	output = new char*[length];
	int *divisorNums=new int[length]; //存储因数
	int divisorNum=0;

	for (int i = 2; i <= length;++i) { 
		if (length%i ==0) {
			output[divisorNum] = new char[length]; //记得分别去new    注意*output = new char[length];仅可初始output[0]
			for (int j = 0; j < length;++j) {
				output[divisorNum][j] = input[j%(length/i)*i + j/(length/i)];
			}
			output[divisorNum][length] = '\0'; //这里记得补充
			divisorNums[divisorNum] = i;
			divisorNum++;
		}
	}
	if (divisorNum) {
		for (int i = 0; i < divisorNum;++i) {
			if (strEqual(output[i],secret)) { //判断字符串是否相等
				cout << divisorNums[i] << endl;
				return 0;
			}
		}
	}
	cout << "No Solution" << endl;
	return 0;
}