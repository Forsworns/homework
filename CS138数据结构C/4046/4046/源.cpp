#include<iostream>
using namespace std;
int matrix[10][10] = { 0 };
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int m, n;
	cin >> m >> n;
	for (int i = 0; i < m;++i) {
		for (int j = 0; j < n;++j) {
			cin >> matrix[i][j];
		}
	}
	cout << n << ' '<<m<<endl;
	for (int i = 0; i < n;++i) {
		for (int j = 0; j < m;++j) {
			cout << matrix[m - j - 1][i];
			if (j!=m-1) {
				cout<<' ';
			}
		}
		if (i!=n-1) { cout << endl; }
	}
}