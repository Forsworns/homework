#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
int prenode[1050]; 
int num[350]={0};
int len = 1;
//2次的power方
void mul2(int power){
    num[0]=1;
    
    for (int i = 0; i < power; ++i)
    {
        int r = 0;
        int ori_len = len;
        for (int j = 0; j < ori_len ; ++j)
        {
            int tmp = num[j]*2 + r;
            r = 0;
            num[j] = ( tmp ) % 10;
          //  cout<<num[j]<<endl;
            if(tmp >= 10){
                r = (tmp)/10 ;//, num[len++]=r;
            }
        }
        if(r!=0)
            num[len++] = r;
    }
}
 
//查
int find(int x){//查找到x所在的子图的根节点 
    int res = x;
    while(res != prenode[res] ){//res是根节点的条件是 res的根节点是本身
        res = prenode[res];
    }
    //压缩路径 让此次查询中的所有涉及的节点的上级节点都是根节点 从而尽量形成只有二级的树结构(不能保证一定是 和输入的边顺序有关)
    while(x != res){
        int pre = prenode[x];
        prenode[x] = res;
        x = pre;
    }
    return res;
}
//并
void join(int x, int y){//把 x 和 y 并到同一子图中
    int rootx = find(x) , rooty = find(y);
    if(rootx != rooty)//x,y不属于同一子图时才有并的必要
        prenode[rootx] = rooty;//属于同一子图
    return;
}
 
 
int main(int argc, char const *argv[])
{
    
    int n,m; 
    cin>>n>>m;
    //初始化n个点 根节点为自身 
    for (int i = 1; i <= n; ++i)
        prenode[i] = i; 
    //调整每个边
    for (int i = 0; i < m; ++i)
    {
        int x,y;
        cin>>x>>y;
        join(x,y);   
    }
    //找连通区域的个数k 
    int k = 0;
    for (int i = 1; i <= n; ++i) 
        if(prenode[i]==i)
            k++;
 
 
    //cout<<k<<endl;
    //cout<<( 1<<(n-k) )<<endl;
    mul2(n-k);
    for (int i = len-1; i >=0; --i)
    {
        cout<<num[i];
    }cout<<endl;
    return 0;
}