#include<iostream>  //�ֵ���
#include<cstring>
#pragma warning (disable:4996)
using namespace std;
int firstArray[30000] = { 0 };
int secondArray[30000] = { 0 };
int tmp[35] = { 0 };
struct node {
	node *left, *right;
	int val;
	node(int value=-1) {
		val = value;
		left = right = NULL;
	}
};
node *headSecond = NULL;
void transform(int num) {	//ת���ɶ����ƴ���tmp
	memset(tmp, 0, sizeof(tmp));	//tmpÿ������Ҫ���
	for (int i = 0; num != 0 && i<31;++i,num /= 2) {   //tmp��һ��ȫ�ֱ�����transform֮��ÿ�ζ�����£����Դ洢������ת��ֵҪ������
		tmp[30-i]=num%2;	//����������ֵķ�Χ�й�Լ��Ҫ31λ������
	}
	/*for (int i = 0; i<31; ++i) {   //tmp��һ��ȫ�ֱ�����transform֮��ÿ�ζ�����£����Դ洢������ת��ֵҪ������
		cout << tmp[i];
	}
	cout << endl;*/
}
void insert(node* &head,int index,int &num) {
	if (index + 1 == 31) {	//��33�㣨��index+1==31��Ҷ�ڵ���ʮ����ֵ,��һ�㣨index+1==-1���գ�����û�о����ֳ��������м�31��Ϊ�����Ƹ�λ
		head->val = num;
		return;
	}
	if (tmp[index+1]==1 && !head->right) {	//��ֱ��������insert��Ϊ���ܽ������޸�ֵ���Ͳ���Ҫnew��
		head->right = new node(-1);
	}
	if (tmp[index+1]==0 && !head->left) {
		head->left = new node(-1);
	}
	
	if (tmp[index+1]==1) {
		insert(head->right,index+1,num);
	}
	if (tmp[index+1]==0) {
		insert(head->left,index+1,num);
	}
}
int dfs(node* &head,int index) {
	if (index+1==31) {
		return head->val;
	}
	if (tmp[index+1]==1) {	//���ʱ���Ǵ浽��ߣ�ȡ��ʱ��ȡ����ΪҪ���
		if (head->left) {	//���������ڵĻ���������һλ�෴������������̰�ķ��������λ���ڶ��㣨index+1==0��ʱ�򣩿�ʼ��λѰ��
			return dfs(head->left, index + 1);
		}
		else {
			return dfs(head->right, index + 1);
		}
	}else {
		if (head->right) {
			return dfs(head->right, index + 1);
		}
		else {
			return dfs(head->left, index + 1);
		}
	}
}
int getMax(int a,int b) {
	if (a>b) return a;
	else return b;
}
int main() {
	freopen("test.txt","r",stdin);
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int firstNum, secondNum;
	int answer = 0;
	headSecond = new node(-1);
	cin >>  firstNum>> secondNum;
	for (int i = 0; i < firstNum;++i) {	
		cin >> firstArray[i];
	}
	for (int i = 0; i < secondNum;++i) {
		cin >> secondArray[i];
	}
	/*for (int i = 0; i < firstNum; ++i) {
		transform(firstArray[i]);
	}*/
	for (int i = 0; i < secondNum;++i) {//transformÿ�ζ�Ҫ��һ��ת����tmp����ǷŽ�ȥ�ֲ�̫�ã���Ϊ�еݹ飬Ū��������ͬ��ͬ����һ�£�
		transform(secondArray[i]);
		insert(headSecond, -1, secondArray[i]);
	}
	for (int i = 0; i < firstNum;++i) {
		transform(firstArray[i]);	//���ݵ�һ�������жϵڶ������ĸ���λ��
		answer = getMax(answer, firstArray[i]^dfs(headSecond,-1));
	}
	cout << answer;
	return 0;
}