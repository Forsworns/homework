#include<iostream>//这是模拟的方法，不如直接计算，但是可以参考一下
#include<queue>
using namespace std;
//这里认为每个人入队时间不计，且是开始一起进入的
int consumerTime[100005] = { 0 };
int currentTime = 0;//当前时间
int timeSum = 0;//所有人排队等待时间和
int consumerNum = 0;
const int serviceTableNum = 3;//服务台数量
class consumer {
public:
	int order;
	int time;
	consumer(int o,int ti) {
		order = o ;
		time = ti;
	}
};
int main() {
	cin >> consumerNum;
	for (int i = 0; i < consumerNum;++i) {
		cin >> consumerTime[i];
	}
	//condition1
	queue<consumer> qC1[serviceTableNum];
	for (int i = 0; i < consumerNum;++i) {
		qC1[i % serviceTableNum].push(consumer(i,consumerTime[i]));
	}
	int frontTime[serviceTableNum] = { 0 };
	for (int i = 0; i < serviceTableNum; ++i) {//获取每个队首顾客的时间
		frontTime[i] = qC1[i].front().time;
		timeSum += qC1[i].front().time*(qC1[i].size() - 1);
	}
	while (!qC1[0].empty() || !qC1[1].empty() || !qC1[2].empty()) {//这里与服务台数量有关
		currentTime++;//模拟每秒判定一次队列
		for (int j = 0; j < serviceTableNum; ++j) {
			if (!qC1[j].empty()) {
				if (frontTime[j]) {
					frontTime[j]--;
				}
				else {
					qC1[j].pop();
					if (!qC1[j].empty()) { 
						frontTime[j] = qC1[j].front().time;
						timeSum += qC1[j].front().time*(qC1[j].size() - 1);
						frontTime[j]--;
					}
				}
			}
			
		}
	}
	currentTime -= 1;//这种判定while的方式最后一定会多算一秒，因为这次只是判定了某个队列为0而没有实际操作
	cout << timeSum << " " << currentTime << endl;
	
	//condition2
	timeSum = 0;
	currentTime = 0;
	queue<consumer> qC2;
	for (int i = 0; i < consumerNum;++i) {
		qC2.push(consumer(i,consumerTime[i]));
	}
	int service[serviceTableNum] = { 0 };
	bool flag[3] = { 0 };
	while (!qC2.empty() || service[0] || service[1] || service[2]) {//这里与服务台数量有关
		currentTime++;
		for (int j = 0; j < serviceTableNum; ++j) {
			if (service[j]) {
				service[j]--;
			}
			else {
				if (!qC2.empty()) {
					service[j] = qC2.front().time;
					qC2.pop();
					service[j]--;
				}
			}
		}
		timeSum += qC2.size();//这种模拟的方法，可以模拟出是每一秒的队列中的人数，不用考虑最小值的问题，因为在模拟过程中就被体现了
	}//这种判定就不会产生第一种排队方式的剩余情况
	cout << timeSum << " " << currentTime << endl;
	
	return 0;
}