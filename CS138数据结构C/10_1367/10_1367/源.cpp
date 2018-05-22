#include<iostream>  //字典树
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
void transform(int num) {	//转化成二进制存入tmp
	memset(tmp, 0, sizeof(tmp));	//tmp每次用需要清空
	for (int i = 0; num != 0 && i<31;++i,num /= 2) {   //tmp是一个全局变量，transform之后每次都会更新，所以存储下来的转化值要尽快用
		tmp[30-i]=num%2;	//与输入的数字的范围有关约需要31位二进制
	}
	/*for (int i = 0; i<31; ++i) {   //tmp是一个全局变量，transform之后每次都会更新，所以存储下来的转化值要尽快用
		cout << tmp[i];
	}
	cout << endl;*/
}
void insert(node* &head,int index,int &num) {
	if (index + 1 == 31) {	//第33层（其index+1==31）叶节点存放十进制值,第一层（index+1==-1）空（于是没有就体现出来），中间31层为二进制各位
		head->val = num;
		return;
	}
	if (tmp[index+1]==1 && !head->right) {	//不直接在这里insert因为可能仅仅是修改值，就不需要new了
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
	if (tmp[index+1]==1) {	//存的时候是存到左边，取得时候取反因为要异或
		if (head->left) {	//左子树存在的话即存在这一位相反的数，这里是贪心法，从最高位即第二层（index+1==0的时候）开始逐位寻找
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
	for (int i = 0; i < secondNum;++i) {//transform每次都要用一次转换到tmp里，但是放进去又不太好，因为有递归，弄个参数不同的同名封一下？
		transform(secondArray[i]);
		insert(headSecond, -1, secondArray[i]);
	}
	for (int i = 0; i < firstNum;++i) {
		transform(firstArray[i]);	//根据第一个数来判断第二个数的各个位置
		answer = getMax(answer, firstArray[i]^dfs(headSecond,-1));
	}
	cout << answer;
	return 0;
}