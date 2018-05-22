#include<iostream>
#pragma warning (disable:4996)
using namespace std;

class mouse {
public:
	int order;
	int score;
	int strength;
	mouse(int ord=0,int sc=0,int st=0):order(ord),score(sc),strength(st) {}
	bool operator<(const mouse &another) {
		return score < another.score;
	}
	bool operator>(const mouse &another) {
		return score > another.score;
	}
	bool operator==(const mouse &another) {
		return score == another.score;
	}
	mouse operator=(const mouse &another) {
		if (this == &another)return *this;
		order = another.order;
		score = another.score;
		strength = another.strength;
		return *this;
	}
};
int mouseNums, roundNums;
mouse mouses[200005];
mouse temp[2000005];
int divide(int start, int end) {
	mouse flag = mouses[start];
	do {
		while (start<end && (mouses[end]<flag || mouses[end] == flag&&mouses[end].order>flag.order)) {
			--end;
		}
		if (start < end) {
			mouses[start] = mouses[end];
			++start;
		}
		while (start<end && (mouses[start]>flag || mouses[start] == flag&&mouses[start].order<flag.order)) {
			++start;
		}
		if (start<end) {
			mouses[end] = mouses[start];
			--end;
		}
	} while (start != end);
	mouses[start] = flag;
	return start;
}
void quickSort(int start, int end) {//第一次用了快排，顺便试一下可以过几个，大的测试点RE而不是TL，应该是递归次数太多栈溢出
	int mid;
	if (start >= end)return;
	mid = divide(start, end);
	quickSort(start, mid - 1);
	quickSort(mid+1, end);
}
void quickSort() {
	quickSort(1,mouseNums*2);
}
void competition() {//类似归并排序中归并的思路，因为所有获胜老鼠之间的先后顺序是不变的，同理
	int *winnerRank,*loserRank,*drawRank;
	winnerRank = new int[mouseNums * 2];//只记录位置，不拷贝mouse减少消耗
	loserRank = new int[mouseNums * 2];
	drawRank = new int[mouseNums * 2];
	int winnerNum = 0, loserNum = 0, drawNum = 0;
	int winnerFlag = 0, loserFlag = 0, drawFlag = 0, tempFlag=0;
	
	for (int j = 1; j <= mouseNums;++j) {//分mouseNums组进行比赛
		if (mouses[2*j-1].strength<mouses[2*j].strength) {
			mouses[2 * j].score += 2;
			winnerRank[winnerNum++] = 2 * j;
			loserRank[loserNum++] = 2 * j - 1;
		}
		else if (mouses[2 * j - 1].strength>mouses[2 * j].strength) {
			mouses[2 * j - 1].score += 2;
			winnerRank[winnerNum++] = 2 * j - 1;
			loserRank[loserNum++] = 2 * j;
		}
		else {
			mouses[2 * j - 1].score++;
			mouses[2 * j].score++;
			drawRank[drawNum++] = 2 * j - 1;
			drawRank[drawNum++] = 2 * j;
		}
	}
	/*for (int j = 1; j <= mouseNums; ++j) {//分mouseNums组进行比赛//一开始这么写写错了，因为搞错了新的次序，其实与原有order没有关系
		if (mouses[2 * j - 1].strength<mouses[2 * j].strength) {
			mouses[2 * j].score += 2;
			winnerRank[winnerNum++] = mouses[2 * j].order;
			loserRank[loserNum++] = mouses[2 * j - 1].order;
		}
		else if (mouses[2 * j - 1].strength>mouses[2 * j].strength) {
			mouses[2 * j - 1].score += 2;
			winnerRank[winnerNum++] = mouses[2 * j - 1].order;
			loserRank[loserNum++] = mouses[2 * j].order;
		}
		else {
			mouses[2 * j - 1].score++;
			mouses[2 * j].score++;
			drawRank[drawNum++] = mouses[2 * j - 1].order;
			drawRank[drawNum++] = mouses[2 * j].order;
		}
	}*/
	while (winnerFlag<winnerNum&&drawFlag<drawNum&& loserFlag<loserNum) {//先判断分数高低，一样的还要判断次序先后
		if (mouses[winnerRank[winnerFlag]]>mouses[drawRank[drawFlag]]) {
			if (mouses[winnerRank[winnerFlag]]>mouses[loserRank[loserFlag]]) {
				temp[tempFlag++] = mouses[winnerRank[winnerFlag++]];
			}
			else if(mouses[winnerRank[winnerFlag]]<mouses[loserRank[loserFlag]]){
				temp[tempFlag++] = mouses[loserRank[loserFlag++]];
			}else{
				temp[tempFlag++] = mouses[winnerRank[winnerFlag]].order<mouses[loserRank[loserFlag]].order ? mouses[winnerRank[winnerFlag++]] : mouses[loserRank[loserFlag++]];
			}
		}
		else if(mouses[winnerRank[winnerFlag]]<mouses[drawRank[drawFlag]]){
			if (mouses[drawRank[drawFlag]]>mouses[loserRank[loserFlag]]) {
				temp[tempFlag++] = mouses[drawRank[drawFlag++]];
			}
			else if(mouses[drawRank[drawFlag]]<mouses[loserRank[loserFlag]]) {
				temp[tempFlag++] = mouses[loserRank[loserFlag++]];
			}
			else {
				temp[tempFlag++] = mouses[drawRank[drawFlag]].order<mouses[loserRank[loserFlag]].order ? mouses[drawRank[drawFlag++]] : mouses[loserRank[loserFlag++]];
			}
		}
		else {
			if (mouses[winnerRank[winnerFlag]]<mouses[loserRank[loserFlag]]) {
				temp[tempFlag++] = mouses[loserRank[loserFlag++]];
			}
			else if (mouses[winnerRank[winnerFlag]]>mouses[loserRank[loserFlag]]) {
				temp[tempFlag++] = mouses[winnerRank[winnerFlag]].order<mouses[drawRank[drawFlag]].order ? mouses[winnerRank[winnerFlag++]] : mouses[drawRank[drawFlag++]];
			}
			else{
				if (mouses[winnerRank[winnerFlag]].order<mouses[drawRank[drawFlag]].order) {
					temp[tempFlag++] = mouses[winnerRank[winnerFlag]].order<mouses[loserRank[loserFlag]].order ? mouses[winnerRank[winnerFlag++]] : mouses[loserRank[loserFlag++]];
				}
				else {
					temp[tempFlag++] = mouses[drawRank[drawFlag]].order<mouses[loserRank[loserFlag]].order ? mouses[drawRank[drawFlag++]] : mouses[loserRank[loserFlag++]];
				}
			}
		}
	}
	while (winnerFlag<winnerNum && drawFlag<drawNum) {
		if (mouses[winnerRank[winnerFlag]]>mouses[drawRank[drawFlag]]) {
			temp[tempFlag++] = mouses[winnerRank[winnerFlag++]];
		}
		else if (mouses[winnerRank[winnerFlag]]<mouses[drawRank[drawFlag]]) {
			temp[tempFlag++] = mouses[drawRank[drawFlag++]];
		}
		else {
			temp[tempFlag++] = mouses[drawRank[drawFlag]].order<mouses[winnerRank[winnerFlag]].order ? mouses[drawRank[drawFlag++]]:mouses[winnerRank[winnerFlag++]] ;
		}
	}
	while (drawFlag<drawNum && loserFlag<loserNum) {
		if (mouses[loserRank[loserFlag]]>mouses[drawRank[drawFlag]]) {
			temp[tempFlag++] = mouses[loserRank[loserFlag++]];
		}
		else if (mouses[loserRank[loserFlag]]<mouses[drawRank[drawFlag]]) {
			temp[tempFlag++] = mouses[drawRank[drawFlag++]];
		}
		else {
			temp[tempFlag++] = mouses[drawRank[drawFlag]].order<mouses[loserRank[loserFlag]].order ? mouses[drawRank[drawFlag++]] : mouses[loserRank[loserFlag++]];
		}
	}
	while (winnerFlag<winnerNum && loserFlag<loserNum){
		if (mouses[winnerRank[winnerFlag]]>mouses[loserRank[loserFlag]]) {
			temp[tempFlag++] = mouses[winnerRank[winnerFlag++]];
		}
		else if (mouses[winnerRank[winnerFlag]]<mouses[loserRank[loserFlag]]) {
			temp[tempFlag++] = mouses[loserRank[loserFlag++]];
		}
		else {
			temp[tempFlag++] = mouses[loserRank[loserFlag]].order<mouses[winnerRank[winnerFlag]].order ? mouses[loserRank[loserFlag++]] : mouses[winnerRank[winnerFlag++]];
		}
	}
	while (winnerFlag<winnerNum) {
		temp[tempFlag++] = mouses[winnerRank[winnerFlag++]];
	}
	while (loserFlag<loserNum) {
		temp[tempFlag++] = mouses[loserRank[loserFlag++]];
	}
	while (drawFlag<drawNum) {
		temp[tempFlag++] = mouses[drawRank[drawFlag++]];
	}

	/*for (int j = 1; j <= mouseNums*2;j++) {//这段赋值注释掉换下面的quickSort解除注释
		mouses[j] = temp[j-1];
	}*/
}

int main() {
	freopen("test.txt","r",stdin);
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	cin >> mouseNums>>roundNums;
	for (int i = 1; i <= mouseNums*2;++i) {
		mouses[i].order = i;
		cin >> mouses[i].score;
	}
	for (int i = 1; i <= mouseNums*2; ++i) {
		cin >> mouses[i].strength;
	}
	quickSort();//先快排一次确定第一轮顺序
	for (int i = 1; i <= roundNums; ++i) {
		competition();
		//quickSort();//上面比赛函数中的注释掉，启用该行用快排解
	}
	for (int i = 1; i <= mouseNums*2; ++i) {
		cout << mouses[i].order << " ";
	}
	return 0;
}