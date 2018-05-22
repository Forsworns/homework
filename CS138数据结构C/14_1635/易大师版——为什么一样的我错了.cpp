#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include "stdlib.h"
using namespace std;

#define maxn 100000
int n;

struct arcnode
{
    int vertex;
    int weight;
    arcnode * next;
    arcnode() {}
    arcnode(int v,int w):vertex(v),weight(w),next(NULL) {}
};

struct vernode
{
    int vex;
    arcnode * firarc;
}Ver[maxn];

void Init()
{
    for(int i = 1; i <= n; i++)
    {
        Ver[i].vex = i;
        Ver[i].firarc = NULL;
    }
}

void Insert2(int a, int b, int w)
{
    arcnode * q = new arcnode(b, w);
    if(Ver[a].firarc == NULL)
        Ver[a].firarc = q;
    else
    {
        arcnode * p = Ver[a].firarc;
        q->next = p;
        Ver[a].firarc = q;
    }
}
struct node
{
    int id;
    int w;
    friend bool operator<(node a, node b)
    {
        return a.w > b.w;
    }
};

#define INF 0xfffff
int parent[maxn];
bool visited[maxn];
node d[maxn];
priority_queue<node> q;
void Dijkstra(int s)
{
    for(int i = 1; i <= n; i++)
    {
        d[i].id = i;
        d[i].w = INF;
        parent[i] = -1;
        visited[i] = false;
    }
    d[s].w = 0;
    q.push(d[s]);
    while(!q.empty())
    {
        node cd = q.top();
        q.pop();
        int u = cd.id;
        if(visited[u])
            continue;
        visited[u] = true;
        arcnode * p = Ver[u].firarc;
        while(p != NULL)
        {
            int v = p->vertex;
            if(!visited[v] && d[v].w > d[u].w+p->weight)
            {
                d[v].w = d[u].w+p->weight;
                parent[v] = u;
                q.push(d[v]);
            }
            p = p->next;
        }
    }
}

int main()
{
    int m, a, b, c, ed;
    scanf("%d%d", &n, &m);
    Init();
    while(m--)
    {
        scanf("%d%d%d", &a, &b, &c);
        Insert2(a, b, c);
        Insert2(b, a, c);
    }
    Dijkstra(1);
    ed=n;
    printf("%d", d[ed].w);
    return 0;
}
