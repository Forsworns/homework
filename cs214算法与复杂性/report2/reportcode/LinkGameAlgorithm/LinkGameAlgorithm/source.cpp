#include <iostream>
#include <string>
#include <ctime>
#include <queue>
#include <set>
#include <utility>
#include <algorithm>
#pragma warning(disable : 4996)
using namespace std;

enum pathType
{
	DFSpath,
	BFSpath,
	enhencedBFSpath,
	classificationPath
};

const string answer[2] = { "不存在路径，无法消除", "存在路径可以消除" };
pair<int, int> startPoint, endPoint;
int **matrix;
bool **visited;
pair<int, int> **path;
int row, col;
int testTimes;
queue<pair<int, int>> BFSqueue;
set<pair<int, int>> linkedPoints, tempStorePoints, templinkedPoints;

void showDFSPath(pair<int, int> now)
{
	while (now != endPoint)
	{
		cout << now.first << "," << now.second << "->";
		now = path[now.first][now.second];
	}
}
void showBFSPath(pair<int, int> now)
{
	if (now != pair<int, int>(0, 0))
	{
		showBFSPath(path[now.first][now.second]);
	}
	else
		return;
	cout << now.first << "," << now.second << "->";
}

void showPath(pathType type)
{
	if (type == BFSpath || type == enhencedBFSpath)
	{
		showBFSPath(path[endPoint.first][endPoint.second]);
		cout << endPoint.first << "," << endPoint.second << endl;
	}
	else if (type == DFSpath || classificationPath)
	{
		showDFSPath(startPoint);
		cout << endPoint.first << "," << endPoint.second << endl;
	}
}

void clear()
{
	testTimes = 0;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j <= col + 1; ++j)
		{
			path[i][j] = pair<int, int>(0, 0);
		}
	}
	for (int i = 0; i <= row + 1; ++i)
	{
		for (int j = 0; j <= col + 1; ++j)
		{
			visited[i][j] = false;
		}
	}
}

bool DFS(int x, int y)
{
	testTimes++;
	visited[x][y] = true;
	if (x == endPoint.first && y == endPoint.second)
	{
		return true;
	}
	else
	{
		if (matrix[x][y] == 0)
		{
			if (x < row + 1 && !visited[x + 1][y])
			{
				if (DFS(x + 1, y))
				{
					path[x][y] = pair<int, int>(x + 1, y);
					return true;
				}
			}
			if (y < col + 1 && !visited[x][y + 1])
			{
				if (DFS(x, y + 1))
				{
					path[x][y] = pair<int, int>(x, y + 1);
					return true;
				}
			}
			if (x > 0 && !visited[x - 1][y])
			{
				if (DFS(x - 1, y))
				{
					path[x][y] = pair<int, int>(x - 1, y);
					return true;
				}
			}
			if (y > 0 && !visited[x][y - 1])
			{
				if (DFS(x, y - 1))
				{
					path[x][y] = pair<int, int>(x, y - 1);
					return true;
				}
			}
		}
		return false;
	}
}

bool DFS()
{
	clear();
	visited[startPoint.first][startPoint.second] = true;
	if (startPoint.first < row + 1)
	{
		if (DFS(startPoint.first + 1, startPoint.second))
		{
			path[startPoint.first][startPoint.second] = pair<int, int>(startPoint.first + 1, startPoint.second);
			return true;
		}
	}
	if (startPoint.second < col + 1)
	{
		if (DFS(startPoint.first, startPoint.second + 1))
		{
			path[startPoint.first][startPoint.second] = pair<int, int>(startPoint.first, startPoint.second + 1);
			return true;
		}
	}
	if (startPoint.first > 0)
	{
		if (DFS(startPoint.first - 1, startPoint.second))
		{
			path[startPoint.first][startPoint.second] = pair<int, int>(startPoint.first - 1, startPoint.second);
			return true;
		}
	}
	if (startPoint.second > 0)
	{
		if (DFS(startPoint.first, startPoint.second - 1))
		{
			path[startPoint.first][startPoint.second] = pair<int, int>(startPoint.first, startPoint.second - 1);
			return true;
		}
	}
	return false;
}

bool BFS(int x, int y)
{
	pair<int, int> child;
	BFSqueue.push(pair<int, int>(x, y));
	while (!BFSqueue.empty())
	{
		child = BFSqueue.front();
		BFSqueue.pop();
		visited[child.first][child.second] = true;
		testTimes++;
		if (child.first == endPoint.first && child.second == endPoint.second)
		{
			return true;
		}
		if (matrix[child.first][child.second] == 0)
		{
			if (child.first < row + 1 && !visited[child.first + 1][child.second])
			{
				BFSqueue.push(pair<int, int>(child.first + 1, child.second));
				path[child.first + 1][child.second] = child;
			}
			if (child.second < col + 1 && !visited[child.first][child.second + 1])
			{
				BFSqueue.push(pair<int, int>(child.first, child.second + 1));
				path[child.first][child.second + 1] = child;
			}
			if (child.first > 0 && !visited[child.first - 1][child.second])
			{
				BFSqueue.push(pair<int, int>(child.first - 1, child.second));
				path[child.first - 1][child.second] = child;
			}
			if (child.second > 0 && !visited[child.first][child.second - 1])
			{
				BFSqueue.push(pair<int, int>(child.first, child.second - 1));
				path[child.first][child.second - 1] = child;
			}
		}
	}
	return false;
}

bool BFS()
{
	clear();
	visited[startPoint.first][startPoint.second] = true;
	if (startPoint.first < row + 1)
	{
		if (BFS(startPoint.first + 1, startPoint.second))
		{
			path[startPoint.first + 1][startPoint.second] = pair<int, int>(startPoint.first, startPoint.second);
			return true;
		}
	}
	if (startPoint.second < col + 1)
	{
		if (BFS(startPoint.first, startPoint.second + 1))
		{
			path[startPoint.first][startPoint.second + 1] = pair<int, int>(startPoint.first, startPoint.second);
			return true;
		}
	}
	if (startPoint.first > 0)
	{
		if (BFS(startPoint.first - 1, startPoint.second))
		{
			path[startPoint.first - 1][startPoint.second] = pair<int, int>(startPoint.first, startPoint.second);
			return true;
		}
	}
	if (startPoint.second > 0)
	{
		if (BFS(startPoint.first, startPoint.second - 1))
		{
			path[startPoint.first][startPoint.second - 1] = pair<int, int>(startPoint.first, startPoint.second);
			return true;
		}
	}
	return false;
}

bool enhencedBFS()
{
	clear();
	visited[startPoint.first][startPoint.second] = true;
	templinkedPoints.insert(pair<int, int>(startPoint.first, startPoint.second));
	for (int loopTimes = 0; loopTimes < 3; ++loopTimes)
	{
		for (set<pair<int, int>>::iterator it = templinkedPoints.begin(); it != templinkedPoints.end(); ++it)
		{
			int i = 1;
			while ((*it).first + i < row + 1 && !visited[(*it).first + i][(*it).second] && matrix[(*it).first + i][(*it).second] == 0)
			{
				tempStorePoints.insert(pair<int, int>((*it).first + i, (*it).second));
				path[(*it).first + i][(*it).second] = *it;
				visited[(*it).first + i][(*it).second] = true;
				++i;
				++testTimes;
			}
			if ((*it).first + i < row + 1 && matrix[(*it).first + i][(*it).second] != 0)
			{
				linkedPoints.insert(pair<int, int>((*it).first + i, (*it).second));
				path[(*it).first + i][(*it).second] = *it;
				visited[(*it).first + i][(*it).second] = true;
			}

			i = 1;
			while ((*it).second + i < col + 1 && !visited[(*it).first][(*it).second + i] && matrix[(*it).first][(*it).second + i] == 0)
			{
				tempStorePoints.insert(pair<int, int>((*it).first, (*it).second + i));
				path[(*it).first][(*it).second + i] = *it;
				visited[(*it).first][(*it).second + i] = true;
				++i;
				++testTimes;
			}
			if ((*it).second + i < col + 1 && matrix[(*it).first][(*it).second + i] != 0)
			{
				linkedPoints.insert(pair<int, int>((*it).first, (*it).second + i));
				path[(*it).first][(*it).second + i] = *it;
				visited[(*it).first][(*it).second + i] = true;
			}

			i = 1;
			while ((*it).first - i > 0 && !visited[(*it).first - i][(*it).second] && matrix[(*it).first - i][(*it).second] == 0)
			{
				tempStorePoints.insert(pair<int, int>((*it).first - i, (*it).second));
				path[(*it).first - i][(*it).second] = *it;
				visited[(*it).first - i][(*it).second] = true;
				++i;
				++testTimes;
			}
			if ((*it).first - i > 0 && matrix[(*it).first - i][(*it).second] != 0)
			{
				linkedPoints.insert(pair<int, int>((*it).first - i, (*it).second));
				path[(*it).first - i][(*it).second] = *it;
				visited[(*it).first - i][(*it).second] = true;
			}

			i = 1;
			while ((*it).second - i > 0 && !visited[(*it).first][(*it).second - i] && matrix[(*it).first][(*it).second - i] == 0)
			{
				tempStorePoints.insert(pair<int, int>((*it).first, (*it).second - i));
				path[(*it).first][(*it).second - i] = *it;
				visited[(*it).first][(*it).second - i] = true;
				++i;
				++testTimes;
			}
			if ((*it).second - i > 0 && matrix[(*it).first][(*it).second - i] != 0)
			{
				linkedPoints.insert(pair<int, int>((*it).first, (*it).second - i));
				path[(*it).first][(*it).second - i] = *it;
				visited[(*it).first][(*it).second - i] = true;
			}
		}
		templinkedPoints.clear();
		for (set<pair<int, int>>::iterator it = tempStorePoints.begin(); it != tempStorePoints.end(); ++it)
		{
			templinkedPoints.insert(*it);
		}
		tempStorePoints.clear();
		for (set<pair<int, int>>::iterator it = templinkedPoints.begin(); it != templinkedPoints.end(); ++it)
		{
			linkedPoints.insert(*it);
		}
		if (linkedPoints.find(pair<int, int>(endPoint.first, endPoint.second)) != linkedPoints.end())
		{
			path[startPoint.first][startPoint.second] = pair<int, int>(0, 0);
			testTimes = linkedPoints.size();
			return true;
		}
	}
	testTimes = linkedPoints.size();
	path[startPoint.first][startPoint.second] = pair<int, int>(0, 0);
	return linkedPoints.find(pair<int, int>(endPoint.first, endPoint.second)) != linkedPoints.end();
}

bool straightLinked(pair<int, int> startNow, pair<int, int> endNow)
{
	testTimes++;
	if (startNow.first == endNow.first)
	{
		for (int i = min(startNow.second, endNow.second) + 1; i <= max(startNow.second, endNow.second) - 1; ++i)
		{
			if (matrix[startNow.first][i] != 0)
				return false;
		}
		path[startNow.first][startNow.second] = endNow;
		return true;
	}
	else if (startNow.second == endNow.second)
	{
		for (int i = min(startNow.first, endNow.first) + 1; i <= max(startNow.first, endNow.first) - 1; ++i)
		{
			if (matrix[i][startNow.second] != 0)
				return false;
		}
		path[startNow.first][startNow.second] = endNow;
		return true;
	}
	else
		return false;
}
bool oneTurnLinked(pair<int, int> startNow, pair<int, int> endNow)
{
	testTimes++;
	if (startNow.first != endNow.first && startNow.second != endNow.second)
	{
		if (matrix[startNow.first][endNow.second] == 0)
		{
			if (straightLinked(startNow, pair<int, int>(startNow.first, endNow.second)) && straightLinked(pair<int, int>(startNow.first, endNow.second), endNow))
			{
				return true;
			}
		}
		if (matrix[endNow.first][startNow.second] == 0)
		{
			if (straightLinked(startNow, pair<int, int>(endNow.first, startNow.second)) && straightLinked(pair<int, int>(endNow.first, startNow.second), endNow))
			{
				return true;
			}
		}
	}
	return false;
}
bool twoTurnLinked(pair<int, int> startNow, pair<int, int> endNow)
{
	testTimes++;
	if (startNow.first == endNow.first)
	{
		for (int i = 0; i <= row + 1; ++i)
		{
			if (i == startNow.first)
				continue;
			if (straightLinked(startNow, pair<int, int>(i, startNow.second)) && oneTurnLinked(pair<int, int>(i, startNow.second), endNow))
			{
				return true;
			}
		}
	}
	else if (startNow.second == endNow.second)
	{
		for (int i = 0; i <= col + 1; ++i)
		{
			if (i == startNow.second)
				continue;
			if (straightLinked(startNow, pair<int, int>(startNow.first, i)) && oneTurnLinked(pair<int, int>(startNow.first, i), endNow))
			{
				return true;
			}
		}
	}
	return false;
}
bool classification()
{
	clear();
	visited[startPoint.first][startPoint.second] = true;
	return straightLinked(startPoint, endPoint) || oneTurnLinked(startPoint, endPoint) || twoTurnLinked(startPoint, endPoint);
}

int main()
{
	bool answerIndex = false;
	freopen("test.txt", "r", stdin);
	cin >> row >> col;
	matrix = new int *[row + 2];
	for (int i = 0; i < row + 2; ++i)
	{
		matrix[i] = new int[col + 2];
	}
	visited = new bool *[row + 2];
	for (int i = 0; i < row + 2; ++i)
	{
		visited[i] = new bool[col + 2];
	}
	path = new pair<int, int> *[row + 2];
	for (int i = 0; i < row + 2; ++i)
	{
		path[i] = new pair<int, int>[col + 2];
	}
	for (int i = 0; i < row + 2; ++i)
	{
		matrix[i][0] = 0;
		matrix[i][col + 1] = 0;
	}
	for (int i = 0; i < col + 2; ++i)
	{
		matrix[0][i] = 0;
		matrix[row + 1][i] = 0;
	}
	for (int i = 1; i <= row; ++i)
	{
		for (int j = 1; j <= col; ++j)
		{
			cin >> matrix[i][j];
		}
	}
	for (int i = 0; i <= row + 1; ++i)
	{
		for (int j = 0; j <= col + 1; ++j)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cin >> startPoint.first >> startPoint.second >> endPoint.first >> endPoint.second;
	cout << "杨培灏516021910233" << endl;
	if (matrix[startPoint.first][startPoint.second] != matrix[endPoint.first][endPoint.second])
	{
		cout << "图形不同不可能消除" << endl;
	}
	else
	{
		answerIndex = DFS();
		cout << "DFS方法" << answer[answerIndex];
		if (answerIndex)
		{
			cout << "测试次数：" << testTimes << endl
				<< "路径：";
			showPath(DFSpath);
		}
		answerIndex = BFS();
		cout << "BFS方法" << answer[answerIndex];
		if (answerIndex)
		{
			cout << "测试次数：" << testTimes << endl
				<< "路径：";
			showPath(BFSpath);
		}
		answerIndex = enhencedBFS();
		cout << "BFS增强方法" << answer[answerIndex];
		if (answerIndex)
		{
			cout << "测试次数：" << testTimes << endl
				<< "路径：";
			showPath(enhencedBFSpath);
		}
		answerIndex = classification();
		cout << "分类方法" << answer[answerIndex];
		if (answerIndex)
		{
			cout << "测试次数：" << testTimes << endl
				<< "路径：";
			showPath(classificationPath);
		}
	}
	return 0;
}