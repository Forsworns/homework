#include<iostream>//����ģ��ķ���������ֱ�Ӽ��㣬���ǿ��Բο�һ��
#include<queue>
using namespace std;
//������Ϊÿ�������ʱ�䲻�ƣ����ǿ�ʼһ������
int consumerTime[100005] = { 0 };
int currentTime = 0;//��ǰʱ��
int timeSum = 0;//�������Ŷӵȴ�ʱ���
int consumerNum = 0;
const int serviceTableNum = 3;//����̨����
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
	for (int i = 0; i < serviceTableNum; ++i) {//��ȡÿ�����׹˿͵�ʱ��
		frontTime[i] = qC1[i].front().time;
		timeSum += qC1[i].front().time*(qC1[i].size() - 1);
	}
	while (!qC1[0].empty() || !qC1[1].empty() || !qC1[2].empty()) {//���������̨�����й�
		currentTime++;//ģ��ÿ���ж�һ�ζ���
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
	currentTime -= 1;//�����ж�while�ķ�ʽ���һ�������һ�룬��Ϊ���ֻ���ж���ĳ������Ϊ0��û��ʵ�ʲ���
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
	while (!qC2.empty() || service[0] || service[1] || service[2]) {//���������̨�����й�
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
		timeSum += qC2.size();//����ģ��ķ���������ģ�����ÿһ��Ķ����е����������ÿ�����Сֵ�����⣬��Ϊ��ģ������оͱ�������
	}//�����ж��Ͳ��������һ���Ŷӷ�ʽ��ʣ�����
	cout << timeSum << " " << currentTime << endl;
	
	return 0;
}