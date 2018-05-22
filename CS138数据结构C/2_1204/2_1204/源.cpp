#pragma warning(disable:4996) //�±�Խ�������һ���ж�����������<0����ôȡ�������е��������Ⱥ������
#include<iostream>
#include <stdio.h>
#include <math.h>
using namespace std;
const int maxRow = 100;
const int maxChar = 2000;
const int addCharMax = 100;
const int numSize = 100;
const int NONE = 0;
const int LIST = 1;
const int DEL = 2;
const int INS = 3;
char temp[maxChar] = { '\0' };//�������ins��ҪaddCharMax������del��ҪmaxChar������ջ���
int length(char row[]) {//�������һ����\0��������صĳ���û�а���\0
	for (int i = 0; i < maxChar; ++i) {
		if (row[i] == '\0') {
			return i;
		}
	}
	return maxChar - 1;
}
void list(int n1, int n2, char text[maxRow][maxChar], int charNumEachRow[], int rowNum) {
	if (n2<n1 || n1 <= 0 || n2 <= 0 || n2>rowNum) {
		cout << "Error!" << endl;
		return;
	}
	for (int i = n1 - 1; i < n2; ++i) {
		for (int j = 0; j < charNumEachRow[i]; ++j) {
			cout << text[i][j];
		}
		cout << endl;
	}
}
void ins(int i, int j, int &rowNum, int charNumEachRow[], char addChar[], char text[maxRow][maxChar]) {//����������������
	int addCharLength = length(addChar);
	if (i <= 0 || j <= 0 || i > rowNum) {
		cout << "Error!" << endl;
		return;
	}
	if (j > charNumEachRow[i - 1] + 1 || addCharLength>addCharMax || addCharLength + charNumEachRow[i - 1]>maxChar) {
		cout << "Error!" << endl;
		return;
	}

	for (int p = 0; p < maxChar; ++p) {
		temp[p] = '\0';
	}
	for (int k = j - 1; k < charNumEachRow[i - 1]; k++) { //�����м���������λ�ú�����ַ�������
		temp[k - j + 1] = text[i - 1][k];
	}
	for (int k = j - 1; k < j + addCharLength - 1; k++) { //�ٰѲ����ַ�����
		text[i - 1][k] = addChar[k - j + 1];
	}
	charNumEachRow[i - 1] += addCharLength;
	for (int k = j + addCharLength - 1; k<charNumEachRow[i - 1] + addCharLength; k++) { //�ٰ���ʱ�����е��ַ�ȡ����
		text[i - 1][k] = temp[k - j - addCharLength + 1];
	}
	return;
}
void del(char text[maxRow][maxChar], int i, int j, int delNum, int rowNum, int charNumEachRow[]) {
	if (i>rowNum || i <= 0 || j <= 0 || delNum < 0) {
		cout << "Error!" << endl;
		return;
	}
	else if (j + delNum >(charNumEachRow[i - 1] + 1)) {//�����ܲ���ȡ��//����ɾ���Ĳ��ܱ�ʣ��������(QAQ����������ط�����һ�죬д����delNum > charNumEachRow[i-1])
		cout << "Error!" << endl;
		return;
	}

	for (int p = 0; p < maxChar; ++p) {
		temp[p] = '\0';
	}
	for (int k = j + delNum - 1; k < charNumEachRow[i - 1]; ++k) {
		temp[k - j - delNum + 1] = text[i - 1][k];
	}
	for (int k = j - 1; k < charNumEachRow[i - 1] - delNum; ++k) {
		text[i - 1][k] = temp[k - j + 1];
	}
	charNumEachRow[i - 1] -= delNum;
}
void quit(char text[maxRow][maxChar], int rowNum, int charNumEachRow[]) {
	for (int i = 0; i < rowNum; ++i) {
		for (int j = 0; j < charNumEachRow[i]; ++j) {
			cout << text[i][j];
		}
		cout << endl;
	}
}
int whichCommand(char command[]) {
	if (command[0] == 'l'&&command[1] == 'i'&&command[2] == 's'&&command[3] == 't') {
		return LIST;
	}
	else if (command[0] == 'd'&&command[1] == 'e'&&command[2] == 'l') {
		return DEL;
	}
	else if (command[0] == 'i'&&command[1] == 'n'&&command[2] == 's') {
		return INS;
	}
	else return NONE;
}
int char2Int(char num[], int size) {
	int temp = 0;
	for (int i = 0; i<size; ++i) {
		temp += (num[i] - '0') * pow(10, size - i - 1);
	}
	return temp;
}
int main() {
	char text[maxRow][maxChar] = { '\0' };
	int rowNum = 0;
	int charNumEachRow[maxRow] = { 0 };
	//freopen("test.txt", "r", stdin);
	for (int i = 0; i < maxRow; i++) {
		cin.get(text[i], maxChar);
		charNumEachRow[i] = length(text[i]);
		cin.get();//�ǵóԵ��������getchar();������
		if (text[i][0] == '*'&&text[i][1] == '*'&&text[i][2] == '*'&&text[i][3] == '*'&&text[i][4] == '*'&&text[i][5] == '*'&&text[i][6] == '\0') {
			rowNum = i; //������ô��rowNum�õ����ǳ�ȥ***�е�������
			break;
		}
	}

	while (true) { // �����ʽ�޶����ÿո�ָ��scanf����????
		char command[maxChar] = { '\0' };
		cin.get(command, maxChar);
		getchar();
		if (!length(command)) {
			cout << "Error!" << endl;
			continue;
		}
		if (command[0] == 'q'&&command[1] == 'u'&&command[2] == 'i'&&command[3] == 't')break;
		switch (whichCommand(command)) {
		case NONE: {
			cout << "Error!" << endl;
			break;
		}
		case LIST: {
			//cout << LIST<<endl;
			int commandLength = length(command);
			char row[numSize] = { '\0' };
			char col[numSize] = { '\0' };
			for (int s = 5; s < commandLength; ++s) {
				if (command[s] == ' ')break;
				row[s - 5] = command[s];
			}
			int rowLength = length(row);
			for (int s = 6 + rowLength; s < commandLength; ++s) {
				if (command[s] == ' ' || command[s] == '\0')break;
				col[s - 6 - rowLength] = command[s];
			}
			int colLength = length(col);
			if (!colLength || !rowLength || colLength + rowLength + 6 != commandLength) {
				cout << "Error!" << endl;
				break;
			}
			int i = char2Int(row, rowLength);
			int j = char2Int(col, colLength);
			list(i, j, text, charNumEachRow, rowNum);
			break;
		}
		case DEL: {
			//cout << DEL << endl;
			int commandLength = length(command);
			char row[numSize] = { '\0' };
			char col[numSize] = { '\0' };
			char delNumChar[numSize] = { '\0' };
			for (int s = 4; s < commandLength; ++s) {
				if (command[s] == ' ')break;
				row[s - 4] = command[s];
			}
			int rowLength = length(row);
			for (int s = 5 + rowLength; s < commandLength; ++s) {
				if (command[s] == ' ')break;
				col[s - 5 - rowLength] = command[s];
			}
			int colLength = length(col);
			for (int s = 6 + rowLength + colLength; s < commandLength; ++s) {
				if (command[s] == ' ' || command[s] == '\0')break;
				delNumChar[s - 6 - rowLength - colLength] = command[s];
			}
			int delLength = length(delNumChar);
			if (!rowLength || !colLength || !delLength || colLength + rowLength + delLength + 6 != commandLength) {
				cout << "Error!" << endl;
				break;
			}
			int delNum = char2Int(delNumChar, delLength);
			int i = char2Int(row, rowLength);
			int j = char2Int(col, colLength);
			del(text, i, j, delNum, rowNum, charNumEachRow);
			break;
		}
		case INS: {
			//cout << INS << endl;
			int commandLength = length(command);
			char addChar[maxChar] = { '\0' };
			char row[numSize] = { '\0' };
			char col[numSize] = { '\0' };
			for (int s = 4; s < commandLength; ++s) {
				if (command[s] == ' ')break;
				row[s - 4] = command[s];
			}
			int rowLength = length(row);
			for (int s = 5 + rowLength; s < commandLength; ++s) {
				if (command[s] == ' ')break;
				col[s - 5 - rowLength] = command[s];
			}
			int colLength = length(col);
			for (int s = 6 + rowLength + colLength; s < commandLength; ++s) {
				if (command[s] == '\0')break;
				addChar[s - 6 - rowLength - colLength] = command[s];
			}
			if (!rowLength || !colLength || !length(addChar) || colLength + rowLength + length(addChar) + 6 != commandLength) {
				cout << "Error!" << endl;
				break;
			}
			int i = char2Int(row, rowLength);
			int j = char2Int(col, colLength);
			ins(i, j, rowNum, charNumEachRow, addChar, text);
			break;
		}
		}
	}
	quit(text, rowNum, charNumEachRow);
	return 0;
}