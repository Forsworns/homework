#include<iostream>
#include<algorithm>
#include<cstdio>
#pragma warning (disable:4996)
using namespace std;
const char MERGE = 'C';
const char DELETE = 'D';
const char QUERY = 'Q';
int maxBox = 1;//这个一开始是1因为每个盒子都是1，写成了0写错了
//不相交集
//parent[i]<0表示盒子存在(是根结点)，否则表示该盒子中的该号糖果被放到了另一个盒子中，使用parent存储它的父盒子，如果是0说明被吃掉了
int parent[500001] = {0};
int boxOfThisNum[500001] = { 0 };//数组的第i项表示有i个糖果的盒子数目
int find(int pos) {
	if (pos == 0) {//这里写parent[pos]==0会出错？
		return 0;
	}
	if (parent[pos] <0) {
		return pos;
	}	
	else {//使用递归进行路径压缩
		return parent[pos] = find(parent[pos]);
	}
}
void unionSweet(int first,int second) {//注意这里传入的参数已经是根结点，所以他的parent[]存的是规模
	if (first == second)return;
	boxOfThisNum[-parent[first]]--;
	boxOfThisNum[-parent[second]]--;
	boxOfThisNum[-(parent[first] + parent[second])]++;
	maxBox = max(maxBox,-(parent[first]+parent[second]));//规模是负数，所以要反向
	if (parent[first] > parent[second]) {
		parent[second] += parent[first];
		parent[first] = second;//根结点用负数存储规模大小，其他节点存的是父亲的位置
	}//树的规模用负数表示，所以小的反而大
	else {
		parent[first] += parent[second];
		parent[second] = first;
	}
}
int main() {//优化后的cin还是比scanf慢很多？？换成cin超时
	//freopen("test.txt","r",stdin);
	//ios::sync_with_stdio(false);
	//std::cin.tie(0);
	int sweetNum, orderNum;
	char order;
	//cin >> sweetNum >> orderNum;
	scanf("%d%d", &sweetNum, &orderNum);
	for (int i = 1; i <= sweetNum;++i) {
		parent[i] = -1;
	}
	boxOfThisNum[1] = sweetNum;//初始时每个盒子里都是1个糖
	for (int i = 0; i < orderNum;++i) {
		scanf("%s", &order);
		//cin >> order;
		if (order == MERGE) {
			int x, y;
			//cin >> x >> y;
			scanf("%d%d", &x, &y);
			x = find(x);
			y = find(y);
			if (x != 0 && y != 0) {
				unionSweet(x,y);
			}
		}
		else if (order == DELETE) {
			int x;
			//cin >> x;
			scanf("%d", &x);
			x = find(x);
			if (x != 0) {
				boxOfThisNum[-parent[x]]--;
				if (-parent[x]==maxBox) {
					for (int j = maxBox; j > 0;--j) {
						if (boxOfThisNum[j]>0) {//在装有某糖数的盒子数中寻找新的最大值
							maxBox = j; 
							break;
						}
					}
				}
				parent[x] = 0;//只需要置零根结点就行，盒子里其他挪过去的自然会在Find的时候因为溯源找到0而被认为是吃掉了
				//但是要讨论一种情况就是吃掉的是最大的那个，那么就要重置最大值即中间的那个判断里的
			}
		}
		else {
			int p;
			//cin >> p;
			scanf("%d", &p);
			for (int i = maxBox; i > 0;--i) {
				p -= boxOfThisNum[i];//从当前存有最多糖的盒子开始减，从大的开始贪心法寻找
				if (p<=0) {
					//cout << i << endl;
					printf("%d\n", i);
					break;
				}
			}
			//if(p>0) cout << 0 << endl;//减去只有一个糖的盒子数目，也始终没有找到
			if (p>0) printf("%d\n", 0);//减去只有一个糖的盒子数目，也始终没有找到
		}
	}
	return 0;
}