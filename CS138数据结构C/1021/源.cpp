#include<iostream>
#include<iomanip>
using namespace std;
int contentHeight = 1;
int main() {
	int N;
	cin >> N;
	int **mountains;
	mountains = new int*[N];
	for (int i = 0; i < N;++i) {
		mountains[i] = new int[N];
	}

	for (int i = 0; i <= (N - 1) / 2;++i) { //顺时针每次遍历填写，注意下标就可以了
		if (N==2*i+1) {
			mountains[i][i] = contentHeight;
		}
		else {
			for (int j = i; j < N - i - 1; ++j) {
				mountains[i][j] = contentHeight;
				++contentHeight;
			}
			for (int j = i; j < N - i - 1; ++j) {
				mountains[j][N - i - 1] = contentHeight;
				++contentHeight;
			}
			for (int j = i; j < N - i - 1; ++j) {
				mountains[N - i - 1][N - j - 1] = contentHeight;
				++contentHeight;
			}
			for (int j = i; j < N - i - 1; ++j) {
				mountains[N - j - 1][i] = contentHeight;
				++contentHeight;
			}
		}	
	}

	for (int i = 0; i < N;++i) {
		for (int j = 0; j < N;++j) {
			cout << setw(6)<<mountains[i][j];
		}
		cout << endl;
	}
	
	return 0;
}