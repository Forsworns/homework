	#include<iostream>
	#include<queue>
	#include<utility>

	#pragma warning (disable:4996)
	using namespace std;
	int cityNum, roadNum,answer;
	const int noRoad = 100000000;
	int dist[1000002];//维护到某个城市的最短距离
	bool known[1000002];//是否到访过该点

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
		cityList[startCity].head = new road(endCity,roadLength,cityList[startCity].head);//无向图须保存两个
		cityList[endCity].head = new road(startCity, roadLength, cityList[endCity].head);
	}
	void dijkstra(int cityNums) {//这里默认从1号城市开始
		road *p;
		int min;//起始点到当前点的最短长度
		int temp;//当前点
		for (int i = 1; i <= cityNums;++i) {
			dist[i] = noRoad;//这个长度表示不存在路径
			known[i] = false;//还未经过
		}
		dist[1] = 0;//这里由于已知1号城市为起点，是对1号城市的初始化
		for (int i = 2; i <= cityNums; ++i) {
			min = noRoad;
			for (int j = 1; j <= cityNums;++j) {
				if (!known[j]&&dist[j]<min) {//寻找具有最短距离的结点
					min = dist[j];
					temp = j;
				}
			}
			known[temp] = true;
			for (p = cityList[temp].head; p!=NULL;p=p->next) {//更新temp邻接点的距离
				if (!known[p->endCityOrder]&&dist[p->endCityOrder]>min+p->length) {
					dist[p->endCityOrder] = min + p->length;
				}
			}
		}
		answer = dist[cityNums];
	}

	typedef pair<int, int> cityNow;//first 源点到该点距离，second 该点序号，pair默认先比较first
	priority_queue<cityNow> cityQue;;   //优先从小到大
	void dijkstraOptimized(int cityNums) {//优先队列优化
		for (int i = 1; i <= cityNums; ++i) {
			dist[i] = noRoad;//这个长度表示不存在路径
			known[i] = false;//还未经过
		}
		dist[1] = 0;//这里由于已知1号城市为起点，是对1号城市的初始化
		cityQue.push(cityNow(0,1));
		while (!cityQue.empty()) {
			cityNow cityPoped = cityQue.top();
			cityQue.pop();
			int temp = cityPoped.second;
			if (known[temp]) {
				continue;
			}
			known[temp] = true;
			for (road *p = cityList[temp].head; p != NULL; p = p->next) {//更新temp邻接点的距离
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
			dist[i] = noRoad;//这个长度表示不存在路径
		}
		dist[1] = 0;//这里由于已知1号城市为起点，是对1号城市的初始化
		cityQue.push(cityNow(0,1));
		while (!cityQue.empty()) {
			cityNow cityPoped = cityQue.top();
			cityQue.pop();
			int temp = cityPoped.second;
			if (dist[temp]<cityPoped.first) {//如果获取到的当前点距离比存储的还要大，那么没有比较意义可以跳过
				continue;
			}
			for (p = cityList[temp].head; p != NULL; p = p->next) {//更新temp邻接点的距离
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