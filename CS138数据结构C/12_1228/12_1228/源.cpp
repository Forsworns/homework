#include<iostream>
#pragma warning (disable:4996)
using namespace std;
const int MAXSIZE = 401;
//long long int Matrix[MAXSIZE][MAXSIZE] = {0};//�洢�󶥵㵽�õ�Ϊֹ��ֵ
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
		int rowSum[MAXSIZE][MAXSIZE] = {0};//����һ����ά����洢ԭ����ı��Σ���ǰ�д�����е�����Ϊֹ�Ĵ�С
		for (int j = i; j <= matrixSize; ++j) {//�̶�������֮��
			int Odd[MAXSIZE] = { 0 }, Even[MAXSIZE] = { 0 };
			for (int k = 1; k <= matrixSize;++k) {//֮���ö�̬�滮��˼�봦����
				rowSum[k][j] = rowSum[k][j - 1] + matrix[k][j];
				if (rowSum[k][j]%2) {//���������ż������Ӱ��֮ǰ�������������תż��ż������棬Ȼ��������������Ҫ+1
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



	/*for (int i = 1; i <= matrixSize; ++i) {//1����0ż
		for (int j = 1; j <= matrixSize; ++j) {//�洢ÿ�е���������ż��
			cin >> matrix[i][j];
			matrix[i][j] = ((matrix[i][j] % 2) + matrix[i - 1][j - 1] - matrix[i - 1][j] - matrix[i][j - 1])%2;
		}
	}
	for (int firstCol = 1; firstCol <= matrixSize; ++firstCol) {
		for (int secondCol = firstCol; secondCol <= matrixSize; ++secondCol) {//�̶�����
			for (int firstRow = 1; firstRow <= matrixSize; ++firstRow) {
				for (int secondRow = firstRow; secondRow <= matrixSize; ++secondRow) {//�����0��1������-1��1����ת���ɼӷ�
					tmpSum = (matrix[secondRow][secondCol] + matrix[firstRow - 1][firstCol - 1] - matrix[secondRow][firstCol - 1] - matrix[firstRow - 1][secondCol])%2;
					if (tmpSum) {
						odd++;//��
					}
					else {
						even++;//ż
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
		for (int j = 1; j <= matrixSize; ++j) {//�洢ÿ�е���������ż��
			cin >> matrix[i][j];
			matrix[i][j] = (0.5 - matrix[i][j] % 2) * 2 * matrix[i-1][j-1] / matrix[i-1][j] / matrix[i][j-1];
		}
	}
	for (int firstCol = 1; firstCol <= matrixSize; ++firstCol) {
		for (int secondCol = firstCol; secondCol <= matrixSize; ++secondCol) {//�̶�����
			for (int firstRow = 1; firstRow <= matrixSize; ++firstRow) {
				for (int secondRow = firstRow; secondRow <= matrixSize; ++secondRow) {//�����0��1������-1��1����ת���ɼӷ�
					tmpSum = matrix[secondRow][secondCol] *matrix[firstRow - 1][firstCol - 1] /matrix[secondRow][firstCol - 1] /matrix[firstRow - 1][secondCol];
					if (tmpSum < 0) {
						odd++;//��
					}
					else {
						even++;//ż
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
	for (int firstCol = 1; firstCol <= matrixSize; ++firstCol) {//O��n^6��->O(n^4)
		for (int secondCol = firstCol; secondCol <= matrixSize;++secondCol) {//�̶�����
			for (int firstRow = 1; firstRow <= matrixSize; ++firstRow) {
				for (int secondRow = firstRow; secondRow <= matrixSize; ++secondRow) {
					tmpSum = Matrix[secondRow][secondCol]+Matrix[firstRow-1][firstCol-1]- Matrix[secondRow][firstCol - 1] - Matrix[firstRow - 1][secondCol];
					if (tmpSum%2) {
						odd++;//��
					}
					else {
						even++;//ż
					}
				}
			}
		}
	}*/
	cout << odd << " " << even;
	return 0;
}