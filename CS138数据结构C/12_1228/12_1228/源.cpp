#include<iostream>
#pragma warning (disable:4996)
using namespace std;
const int MAXSIZE = 401;
//long long int Matrix[MAXSIZE][MAXSIZE] = {0};//存储左顶点到该点为止的值
int matrix[MAXSIZE][MAXSIZE] = {0};
int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	int matrixSize = 0;
	cin >> matrixSize;
	int tmpSum = 0;
	long long int odd = 0,even=0;

	for (int i = 1; i <= matrixSize; ++i) {
		for (int j = 1; j <= matrixSize; ++j) {
		cin >> matrix[i][j];
		}
	}
	for (int i = 1; i <= matrixSize; ++i) {
		int rowSum[MAXSIZE][MAXSIZE] = {0};//创建一个二维数组存储原矩阵的变形，当前行从起点列到该列为止的大小
		for (int j = i; j <= matrixSize; ++j) {//固定在两列之间
			int Odd[MAXSIZE] = { 0 }, Even[MAXSIZE] = { 0 };
			for (int k = 1; k <= matrixSize;++k) {//之后用动态规划的思想处理行
				rowSum[k][j] = rowSum[k][j - 1] + matrix[k][j];
				if (rowSum[k][j]%2) {//如果这行是偶数，不影响之前的情况，否则奇转偶，偶变成了奇，然后奇数的数量还要+1
					Odd[k] = Even[k - 1]+1;
					Even[k] = Odd[k - 1];
					odd += Odd[k];
					even += Even[k];
				}
				else {
					Odd[k] = Odd[k - 1];
					Even[k] = Even[k - 1]+1;
					odd += Odd[k];
					even += Even[k];
				}
			}
		}
	}



	/*for (int i = 1; i <= matrixSize; ++i) {//1奇数0偶
		for (int j = 1; j <= matrixSize; ++j) {//存储每行到该数的奇偶性
			cin >> matrix[i][j];
			matrix[i][j] = ((matrix[i][j] % 2) + matrix[i - 1][j - 1] - matrix[i - 1][j] - matrix[i][j - 1])%2;
		}
	}
	for (int firstCol = 1; firstCol <= matrixSize; ++firstCol) {
		for (int secondCol = firstCol; secondCol <= matrixSize; ++secondCol) {//固定列数
			for (int firstRow = 1; firstRow <= matrixSize; ++firstRow) {
				for (int secondRow = firstRow; secondRow <= matrixSize; ++secondRow) {//如果用0、1而不是-1，1可以转化成加法
					tmpSum = (matrix[secondRow][secondCol] + matrix[firstRow - 1][firstCol - 1] - matrix[secondRow][firstCol - 1] - matrix[firstRow - 1][secondCol])%2;
					if (tmpSum) {
						odd++;//奇
					}
					else {
						even++;//偶
					}
				}
			}
		}
	}*/

	/*for (int i = 0; i <= matrixSize;++i) {
		matrix[i][0] = 1;
	}
	for (int i = 1; i <= matrixSize;++i) {
		matrix[0][i] = 1;
	}
	for (int i = 1; i <= matrixSize; ++i) {
		for (int j = 1; j <= matrixSize; ++j) {//存储每行到该数的奇偶性
			cin >> matrix[i][j];
			matrix[i][j] = (0.5 - matrix[i][j] % 2) * 2 * matrix[i-1][j-1] / matrix[i-1][j] / matrix[i][j-1];
		}
	}
	for (int firstCol = 1; firstCol <= matrixSize; ++firstCol) {
		for (int secondCol = firstCol; secondCol <= matrixSize; ++secondCol) {//固定列数
			for (int firstRow = 1; firstRow <= matrixSize; ++firstRow) {
				for (int secondRow = firstRow; secondRow <= matrixSize; ++secondRow) {//如果用0、1而不是-1，1可以转化成加法
					tmpSum = matrix[secondRow][secondCol] *matrix[firstRow - 1][firstCol - 1] /matrix[secondRow][firstCol - 1] /matrix[firstRow - 1][secondCol];
					if (tmpSum < 0) {
						odd++;//奇
					}
					else {
						even++;//偶
					}
				}
			}
		}
	}*/



	/*for (int i = 1; i <= matrixSize;++i) {
		for (int j = 1; j <= matrixSize;++j) {
			cin >> Matrix[i][j];
			Matrix[i][j] = Matrix[i][j]+Matrix[i][j - 1]+ Matrix[i-1][j]- Matrix[i-1][j-1];
		}
	}
	for (int firstCol = 1; firstCol <= matrixSize; ++firstCol) {//O（n^6）->O(n^4)
		for (int secondCol = firstCol; secondCol <= matrixSize;++secondCol) {//固定列数
			for (int firstRow = 1; firstRow <= matrixSize; ++firstRow) {
				for (int secondRow = firstRow; secondRow <= matrixSize; ++secondRow) {
					tmpSum = Matrix[secondRow][secondCol]+Matrix[firstRow-1][firstCol-1]- Matrix[secondRow][firstCol - 1] - Matrix[firstRow - 1][secondCol];
					if (tmpSum%2) {
						odd++;//奇
					}
					else {
						even++;//偶
					}
				}
			}
		}
	}*/
	cout << odd << " " << even;
	return 0;
}