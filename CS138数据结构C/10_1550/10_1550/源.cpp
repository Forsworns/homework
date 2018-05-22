#include<iostream>
#include<cmath>
#include<algorithm>
#pragma warning (disable:4996)
int height[1000];
using namespace std;
long long int getMiddle(int left_pos,int right_pos,int maxHeight) {
	if (left_pos>right_pos||maxHeight==0) {
		return 0;
	}
	long long int partialSum = 0;
	for (int k = left_pos; k <= right_pos;++k) {
		partialSum += height[k];
	}
	return maxHeight*(right_pos - left_pos + 1)- partialSum;
}

int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);	//�ʼ��ȡ���ˣ�����Ҫ��cin.get()��������
	std::cin.tie(0);
	int n;
	long long int waterAmount=0;
	cin >> n;
	char c;
	for (int i = 0; i < n;++i) {
		cin >> c>>height[i];
	}
	/*for (int i = 0; i < n; ++i) {
	cout << height[i];
	}*/

	//cout << trapRainWater(height,n);
	
	/*int maxPos = 0;int waterHere=0;
	for (int i = 0; i < n;++i) {	//�ҵ���ߵ�
		if (height[i]>waterHere) {
			waterHere = height[i];
			maxPos = i;
		}
	}
	waterHere = 0;
	for (int i = 0; i < maxPos;++i) {
		if (waterHere>height[i]) {
			waterAmount += waterHere - height[i];
		}
		waterHere = max(waterHere,height[i]);
	}//����ߵ����Ķ�ȡ�������߶���ߵĸ߶�
	waterHere = 0;
	for (int i = n - 1; i > maxPos;--i) {
		if (waterHere>height[i]) {
			waterAmount += waterHere - height[i];
		}
		waterHere = max(waterHere, height[i]);
	}
	cout << waterAmount;//����ߵ��Ҳ�Ķ�ȡ�����Ҳ���ߵĸ߶�*/
	
	int left_bound_height = 0, right_bound_height = 0, left_bound_pos = 0, right_bound_pos = 0;
	for (int i = 0; i < n;++i) {
		//cout << "left_bound_height" << " " << left_bound_height << " " << "left_bound_pos" << " " << left_bound_pos << endl;
		//cout << "right_bound_height" << " " << right_bound_height << " " << "right_bound_pos" << " " << right_bound_pos << endl;
		if (right_bound_height >= left_bound_height) {
			if (left_bound_pos != right_bound_pos) { 
				waterAmount += getMiddle(left_bound_pos+1,right_bound_pos-1,left_bound_height); 
			}
			left_bound_height = right_bound_height;
			left_bound_pos = right_bound_pos;
			right_bound_height = height[i];
			right_bound_pos = i;
		}
		else {
			right_bound_height = height[i];
			right_bound_pos = i;
		}
		//cout << "waterAmount" << waterAmount << endl;
		//cout << "left_bound_height" << " "<<left_bound_height << " " << "left_bound_pos" << " " << left_bound_pos << endl;
		//cout << "right_bound_height" << " " << right_bound_height << " " << "right_bound_pos" << " " << right_bound_pos << endl<<endl;
	}
	if (right_bound_height < left_bound_height) {	//������һ���֣���Ϊ֮ǰ��Լ�����ǵ����Ҳ�����߲ż��㣬ע�����һ�ε��Ҳ໹û�м���
		int	last_left_bound_pos = left_bound_pos;
		left_bound_height = right_bound_height;
		left_bound_pos = right_bound_pos;
		for (int j = n - 1; j >= last_left_bound_pos - 1; --j) {
			//cout << "left_bound_height" << " " << left_bound_height << " " << "left_bound_pos" << " " << left_bound_pos << endl;
			//cout << "right_bound_height" << " " << right_bound_height << " " << "right_bound_pos" << " " << right_bound_pos << endl;
			if (left_bound_height >= right_bound_height) {	//�������һ���ַ���֮ǰ��˳�򣬵���������
				if (left_bound_pos != right_bound_pos) {
					waterAmount += getMiddle(left_bound_pos + 1, right_bound_pos - 1, right_bound_height);
				}
				right_bound_height = left_bound_height;
				right_bound_pos = left_bound_pos;
				left_bound_height = height[j];
				left_bound_pos = j;
			}
			else {
				left_bound_height = height[j];
				left_bound_pos = j;
			}
			//cout << "waterAmount" << waterAmount << endl;
			//cout << "left_bound_height" << " " << left_bound_height << " " << "left_bound_pos" << " " << left_bound_pos << endl;
			//cout << "right_bound_height" << " " << right_bound_height << " " << "right_bound_pos" << " " << right_bound_pos << endl << endl;
		}
	}
	if (right_bound_height >= left_bound_height) {
		if (left_bound_pos != right_bound_pos) {
			waterAmount += getMiddle(left_bound_pos + 1, right_bound_pos - 1, left_bound_height);
		}
	}
	cout << waterAmount;
	return 0;
}