#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<iostream> 
#include<algorithm> 
#include<map> 
#include<queue> 
#include<math.h> 
using namespace std;

const int inf = 0x7f7f7f7f;
int mp[110][110];
int G[120][120];
int visit[120][120];
int gg[120][120];
int Hash[110];
int start, End, steps, sum, maxn, bao, m_bao, tt;
int xx[] = { -1, 0, 0, 1 };
int yy[] = { 0, 1, -1, 0 };

int N, M;

struct node
{
	int x, y, step;
};


int jugde(int x, int y)
{
	if (x < 1 || x > N || y < 1 || y > M)
		return 0;
	return 1;
}


void bfs()
{
	queue<node>q;
	memset(visit, 0, sizeof(visit));
	node p;
	p.x = start;
	p.y = End;
	p.step = 0;
	q.push(p);
	while (!q.empty())
	{
		node px = q.front();
		q.pop();
		int x = px.x;
		int y = px.y;
		visit[x][y] = 1;
		int dis = px.step;
		if (mp[x][y] == 1 || mp[x][y] == 2)
		{
			G[gg[start][End]][gg[x][y]] = dis;

		}
		for (int i = 0; i < 4; i++)
		{
			int x1 = xx[i] + x;
			int y1 = yy[i] + y;
			if (mp[x1][y1] != -1 && jugde(x1, y1) && !visit[x1][y1])
			{
				node py;
				py.x = x1;
				py.y = y1;
				py.step = dis + 1;
				visit[x1][y1] = 1;
				q.push(py);

			}

		}

	}

}


void DFS(int x, int  m, int s)
{

	for (int i = 1; i <= tt; i++)
		if (G[x][i] != inf && G[x][i] != 0 && !Hash[i])
		{
			if (m + 1 == m_bao)
			{
				bao = min(bao, s + G[x][i]);
			}
			Hash[x] = 1;
			DFS(i, m + 1, s + G[x][i]);
			Hash[x] = 0;
		}

}


int main()
{
		scanf("%d%d", &N, &M);
		tt = 0;
		steps = 0;
		sum = 0;
		maxn = max(N, M);
		bao = inf;
		m_bao = 0;
		int p, q;
		memset(Hash, 0, sizeof(Hash));
		for (int i = 1; i <= 110; i++)
			for (int j = 1; j <= 110; j++)
				G[i][j] = (i == j) ? 0 : inf;
		for (int i = 1; i <= N; i++)
			for (int j = 1; j <= M; j++)
			{
				scanf("%d", &mp[i][j]);
				if (mp[i][j] == 1)
				{
					m_bao++;
					gg[i][j] = ++tt;
				}
				if (mp[i][j] == 2)
				{
					p = i;
					q = j;
					gg[i][j] = ++tt;

				}
			}

		for (int i = 1; i <= N; i++)
			for (int j = 1; j <= M; j++)
			{
				if (mp[i][j] == 2 || mp[i][j] == 1)
				{
					start = i;
					End = j;
					bfs();
				}
			}
		DFS(gg[p][q], 0, 0);
		if (bao == inf)
			puts("-1");
		else
			printf("%d\n", bao);
	return 0;
}