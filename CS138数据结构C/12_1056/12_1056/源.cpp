#include<iostream>
#include<algorithm>
#include<cstdio>
#pragma warning (disable:4996)
using namespace std;
const char MERGE = 'C';
const char DELETE = 'D';
const char QUERY = 'Q';
int maxBox = 1;//���һ��ʼ��1��Ϊÿ�����Ӷ���1��д����0д����
//���ཻ��
//parent[i]<0��ʾ���Ӵ���(�Ǹ����)�������ʾ�ú����еĸú��ǹ����ŵ�����һ�������У�ʹ��parent�洢���ĸ����ӣ������0˵�����Ե���
int parent[500001] = {0};
int boxOfThisNum[500001] = { 0 };//����ĵ�i���ʾ��i���ǹ��ĺ�����Ŀ
int find(int pos) {
	if (pos == 0) {//����дparent[pos]==0�����
		return 0;
	}
	if (parent[pos] <0) {
		return pos;
	}	
	else {//ʹ�õݹ����·��ѹ��
		return parent[pos] = find(parent[pos]);
	}
}
void unionSweet(int first,int second) {//ע�����ﴫ��Ĳ����Ѿ��Ǹ���㣬��������parent[]����ǹ�ģ
	if (first == second)return;
	boxOfThisNum[-parent[first]]--;
	boxOfThisNum[-parent[second]]--;
	boxOfThisNum[-(parent[first] + parent[second])]++;
	maxBox = max(maxBox,-(parent[first]+parent[second]));//��ģ�Ǹ���������Ҫ����
	if (parent[first] > parent[second]) {
		parent[second] += parent[first];
		parent[first] = second;//������ø����洢��ģ��С�������ڵ����Ǹ��׵�λ��
	}//���Ĺ�ģ�ø�����ʾ������С�ķ�����
	else {
		parent[first] += parent[second];
		parent[second] = first;
	}
}
int main() {//�Ż����cin���Ǳ�scanf���ܶࣿ������cin��ʱ
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
	boxOfThisNum[1] = sweetNum;//��ʼʱÿ�������ﶼ��1����
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
						if (boxOfThisNum[j]>0) {//��װ��ĳ�����ĺ�������Ѱ���µ����ֵ
							maxBox = j; 
							break;
						}
					}
				}
				parent[x] = 0;//ֻ��Ҫ����������У�����������Ų��ȥ����Ȼ����Find��ʱ����Ϊ��Դ�ҵ�0������Ϊ�ǳԵ���
				//����Ҫ����һ��������ǳԵ����������Ǹ�����ô��Ҫ�������ֵ���м���Ǹ��ж����
			}
		}
		else {
			int p;
			//cin >> p;
			scanf("%d", &p);
			for (int i = maxBox; i > 0;--i) {
				p -= boxOfThisNum[i];//�ӵ�ǰ��������ǵĺ��ӿ�ʼ�����Ӵ�Ŀ�ʼ̰�ķ�Ѱ��
				if (p<=0) {
					//cout << i << endl;
					printf("%d\n", i);
					break;
				}
			}
			//if(p>0) cout << 0 << endl;//��ȥֻ��һ���ǵĺ�����Ŀ��Ҳʼ��û���ҵ�
			if (p>0) printf("%d\n", 0);//��ȥֻ��һ���ǵĺ�����Ŀ��Ҳʼ��û���ҵ�
		}
	}
	return 0;
}