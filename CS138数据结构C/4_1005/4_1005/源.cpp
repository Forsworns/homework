#pragma warning(disable:4996)
#include<iostream>
using namespace std;
bool isLegal(int sudoku[9][9]) {
	for (int p = 0; p < 9; ++p) { //¼ì²âÐÐ
		int testArray[9] = { 1,2,3,4,5,6,7,8,9 };
		for (int q = 0; q < 9; ++q) {
			int position = sudoku[p][q] - 1;
			if (testArray[position] == 0) {
				return false;
			}
			testArray[position] = 0;
		}
	}
	for (int p = 0; p < 9; ++p) { //¼ì²âÁÐ
		int testArray[9] = { 1,2,3,4,5,6,7,8,9 };
		for (int q = 0; q < 9; ++q) {
			int position = sudoku[q][p] - 1;
			if (testArray[position] == 0) {
				return false;
			}
			testArray[position] = 0;
		}
	}
	for (int p = 0; p < 9; ++p) { //¼ì²â¾Å¹¬¸ñ
		int testArray[9] = { 1,2,3,4,5,6,7,8,9 };
		for (int q = 0; q < 9; ++q) {
			int position = sudoku[3*(p/3)+q/3][3*(p%3)+q%3] - 1;
			if (testArray[position] == 0) {
				return false;
			}
			testArray[position] = 0;
		}
	}
}
int main() {
	//freopen("test.txt", "r", stdin);
	int n;	
	bool *answer;
	/*for (int p = 0; p < 9; ++p) {
		for (int q = 0; q < 9; ++q) {
			cout << "x" << 3 * (p / 3) + q / 3 << "y" << 3 * (p % 3) + q % 3 << endl;
		}
	}*/
	cin >> n;
	answer = new bool[n];
	int sudoku[9][9] = {0};
	for (int i = 0; i < n;++i) {
		for (int p = 0; p < 9;++p) {
			for (int q = 0; q < 9;++q) {
				cin >> sudoku[p][q];
			}
		}
		answer[i] = isLegal(sudoku);
	}
	for (int i = 0; i < n; ++i) {
		if (answer[i]) {
			cout << "Right" << endl;
		}
		else cout << "Wrong" << endl;
	}

	return 0;
}