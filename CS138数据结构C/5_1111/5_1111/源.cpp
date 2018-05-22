#pragma warning(disable:4996)//˳��ṹ
#include<iostream>//�����±�,�������2*father,�Ҷ�����2*father+1���������1
#include<cmath>
using namespace std;
char preOrder[1000] = {'\0'};
char midOrder[1000] = {'\0'};
char answer[1000] = { '\0' };
int outputNum=0;
const char nullFlag = '0';
int strlength(char string[1000]){
	int i = 0;
	while (string[i]!='\0') {
		++i;
	}
	return i;
}
class outOfBound{};
class illegalSize{};
template <class elemType>
class vector {
private:
	elemType *data;
	int currentLength;
	int maxSize;
	void doubleSpace() {
		elemType *tmp = data;
		maxSize *= 2;
		data = new elemType[maxSize];
		for (int i = 0; i < currentLength; ++i) {
			data[i] = tmp[i];
		}
		delete[]tmp;
	}
public:
	vector(int initSize = 10) {
		if (initSize <= 0)throw illegalSize();
		data = new elemType[initSize];
		maxSize = initSize;
		currentLength = 0;
	}
	~vector() {
		//delete[]data;
	}
	void clear() {
		currentLength = 0;
	}
	vector &splice(vector &another, int start, int end) {//��ȡ��start��end-1��ע��Ҫ���µ�vector���汻splice������another��Ƭ��
		for (int i = start; i < end;++i) {
			data[currentLength++] = another.visit(i);
		}
		return *this;
	}
	int size() const {
		return currentLength;
	}
	int getTargetPos(elemType target) { //���������⣬���ڵ��õ�ʱ��һ�������ҵ�����
		for (int i = 0; i<currentLength; ++i) {
			if (data[i] == target) {
				return i;
			}
		}
		return -1;
	}
	void push(elemType x) {
		if (currentLength == maxSize) {
			doubleSpace();
		}
		data[currentLength++] = x;
	}
	elemType visit(int i)const {
		if (i<0 || i>currentLength - 1) {
			throw outOfBound();
		}
		return data[i];
	}
	bool hasLeftSon(elemType root) { //������ֻ�����������ж����Ҷ���
		if (currentLength<=1)return false;
		else return data[0] != root;
	}
	bool hasRightSon(elemType root) {
		if (currentLength <= 1)return false;
		else if (getTargetPos(root)==currentLength-1)return false;
		else return true;
	}
	bool isEmpty() {
		return currentLength==0;
	}
};

void store(int pos,char str) {
	answer[pos] = str;
}
void solve(int start, int end, int storePos, vector<char> preVec, vector<char> midVec) { //startΪ0,endΪpreVec.size()
	char rootData=nullFlag;
	if (preVec.size()>=1) {
		rootData = preVec.visit(start);//����ǰ��ȥ�洢�����򲻴洢
		store(storePos, rootData);
		int rootInMid = midVec.getTargetPos(rootData);
		vector<char>preleft, preright, midleft, midright;
		if (midVec.hasLeftSon(rootData)) {
			preleft.splice(preVec, start + 1, rootInMid + 1);
			midleft.splice(midVec, start, rootInMid);
		}
		if (midVec.hasRightSon(rootData)) {
			preright.splice(preVec, rootInMid + 1, end);
			midright.splice(midVec, rootInMid + 1, end);
		}
		//cout << preleft.size() << ' ' << preright.size() << ' '<< rootInMid <<endl;
		solve(start, preleft.size(), storePos * 2 + 1, preleft, midleft);
		solve(start, preright.size(), storePos * 2 + 2, preright, midright);
	}
	else {
		store(storePos, rootData);
	}
}
int main() {
	freopen("test.txt", "r", stdin);
	int start = 0;
	cin >> preOrder >> midOrder;
	int inputNum = strlength(preOrder);
	vector<char> preVec(inputNum),midVec(inputNum);
	for (int i = 0; i < inputNum;++i) {
		preVec.push(preOrder[i]);
		midVec.push(midOrder[i]);
	}
	solve(start,inputNum, start,preVec,midVec);
	for (int i = 0;; ++i) {
		if (answer[i] >= 'A'&&answer[i] <= 'Z') { ++outputNum; }
		else { if (answer[i] != nullFlag) { answer[i] = nullFlag; } }
		if (outputNum == inputNum) break;
	}
	outputNum = 0;
	for (int i = 0;; ++i) {
		if (answer[i] == nullFlag) {
			cout << "NULL" << ' ';
		}
		else {
			cout << answer[i] << ' ';
			++outputNum;
			if (outputNum == inputNum) break;
		}
	}
	return 0;
}