	#include<iostream>
	#include<queue>
	#include<utility>

	#pragma warning (disable:4996)
	using namespace std;
	int cityNum, roadNum,answer;
	const int noRoad = 100000000;
	int dist[1000002];//ά����ĳ�����е���̾���
	bool known[1000002];//�Ƿ񵽷ù��õ�

	struct road {
		int endCityOrder;
		int length;
		road *next;
		road(int ci,int le,road *ne=NULL):endCityOrder(ci),length(le),next(ne) {}
		road():endCityOrder(0),length(noRoad),next(NULL){}
	};
	struct city {
		road *head;
		city(road *h=NULL) {
			head = h;
		}
	};
	city *cityList;

	void addTheRoad(int startCity,int endCity,int roadLength) {
		cityList[startCity].head = new road(endCity,roadLength,cityList[startCity].head);//����ͼ�뱣������
		cityList[endCity].head = new road(startCity, roadLength, cityList[endCity].head);
	}
	void dijkstra(int cityNums) {//����Ĭ�ϴ�1�ų��п�ʼ
		road *p;
		int min;//��ʼ�㵽��ǰ�����̳���
		int temp;//��ǰ��
		for (int i = 1; i <= cityNums;++i) {
			dist[i] = noRoad;//������ȱ�ʾ������·��
			known[i] = false;//��δ����
		}
		dist[1] = 0;//����������֪1�ų���Ϊ��㣬�Ƕ�1�ų��еĳ�ʼ��
		for (int i = 2; i <= cityNums; ++i) {
			min = noRoad;
			for (int j = 1; j <= cityNums;++j) {
				if (!known[j]&&dist[j]<min) {//Ѱ�Ҿ�����̾���Ľ��
					min = dist[j];
					temp = j;
				}
			}
			known[temp] = true;
			for (p = cityList[temp].head; p!=NULL;p=p->next) {//����temp�ڽӵ�ľ���
				if (!known[p->endCityOrder]&&dist[p->endCityOrder]>min+p->length) {
					dist[p->endCityOrder] = min + p->length;
				}
			}
		}
		answer = dist[cityNums];
	}

	typedef pair<int, int> cityNow;//first Դ�㵽�õ���룬second �õ���ţ�pairĬ���ȱȽ�first
	priority_queue<cityNow> cityQue;;   //���ȴ�С����
	void dijkstraOptimized(int cityNums) {//���ȶ����Ż�
		for (int i = 1; i <= cityNums; ++i) {
			dist[i] = noRoad;//������ȱ�ʾ������·��
			known[i] = false;//��δ����
		}
		dist[1] = 0;//����������֪1�ų���Ϊ��㣬�Ƕ�1�ų��еĳ�ʼ��
		cityQue.push(cityNow(0,1));
		while (!cityQue.empty()) {
			cityNow cityPoped = cityQue.top();
			cityQue.pop();
			int temp = cityPoped.second;
			if (known[temp]) {
				continue;
			}
			known[temp] = true;
			for (road *p = cityList[temp].head; p != NULL; p = p->next) {//����temp�ڽӵ�ľ���
				if (!known[p->endCityOrder] && dist[p->endCityOrder]>dist[temp] + p->length) {
					dist[p->endCityOrder] =  dist[temp]+ p->length;
					cityQue.push(cityNow(dist[p->endCityOrder],p->endCityOrder));
				}
			}
		}
		answer = dist[cityNums];
	}
	/*void dijkstraOptimized(int cityNums) {
		road *p;
		for (int i = 1; i <= cityNums; ++i) {
			dist[i] = noRoad;//������ȱ�ʾ������·��
		}
		dist[1] = 0;//����������֪1�ų���Ϊ��㣬�Ƕ�1�ų��еĳ�ʼ��
		cityQue.push(cityNow(0,1));
		while (!cityQue.empty()) {
			cityNow cityPoped = cityQue.top();
			cityQue.pop();
			int temp = cityPoped.second;
			if (dist[temp]<cityPoped.first) {//�����ȡ���ĵ�ǰ�����ȴ洢�Ļ�Ҫ����ôû�бȽ������������
				continue;
			}
			for (p = cityList[temp].head; p != NULL; p = p->next) {//����temp�ڽӵ�ľ���
				if (dist[p->endCityOrder]>cityPoped.first + p->length) {
					dist[p->endCityOrder] =  cityPoped.first+ p->length;
					cityQue.push(cityNow(dist[p->endCityOrder],p->endCityOrder));
				}
			}
		}
		answer = dist[cityNums];
	}*/
	int main() {
		freopen("test.txt","r",stdin);
		//ios::sync_with_stdio(false);
		//std::cin.tie(0);
		cin >> cityNum >> roadNum;
		cityList = new city[cityNum+1];
		int startCity, endCity, roadLength;
		for (int i = 0; i < roadNum;++i) {
			scanf("%d%d%d", &startCity, &endCity, &roadLength);
			addTheRoad(startCity, endCity, roadLength);
		}
		dijkstraOptimized(cityNum);
		cout << answer;
		return 0;
	}