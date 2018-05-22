#pragma warning(disable:4996)
#include<iostream>
using namespace std;

int sizeOfStr(char *input) { //��ȡ����
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
	int *divisorNums=new int[length]; //�洢����
	int divisorNum=0;

	for (int i = 2; i <= length;++i) { 
		if (length%i ==0) {
			output[divisorNum] = new char[length]; //�ǵ÷ֱ�ȥnew    ע��*output = new char[length];���ɳ�ʼoutput[0]
			for (int j = 0; j < length;++j) {
				output[divisorNum][j] = input[j%(length/i)*i + j/(length/i)];
			}
			output[divisorNum][length] = '\0'; //����ǵò���
			divisorNums[divisorNum] = i;
			divisorNum++;
		}
	}
	if (divisorNum) {
		for (int i = 0; i < divisorNum;++i) {
			if (strEqual(output[i],secret)) { //�ж��ַ����Ƿ����
				cout << divisorNums[i] << endl;
				return 0;
			}
		}
	}
	cout << "No Solution" << endl;
	return 0;
}