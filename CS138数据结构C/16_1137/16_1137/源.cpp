#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
int n,m,f[20010];
long long number[20010];
struct node
{
    int u,v;
    long long z;
}a[20010];
bool cmp(node a,node b)
{
    if (a.z<b.z) return true;
    return false;
}
int get(int x)
{
    if (f[x]==x) return x;else f[x]=get(f[x]);
    return f[x];
}
int main()
{
int runTimes;
cin>>runTimes;
for(int i=0;i<runTimes;++i){
    scanf("%d",&n);
    long long ans=0;
    for (int i=1;i<=n-1;i++) 
    {
        scanf("%d%d%lld",&a[i].u,&a[i].v,&a[i].z);
        ans+=a[i].z; 
    }
    sort(a+1,a+n,cmp);
    for (int i=1;i<=n;i++) f[i]=i,number[i]=1;
    for (int i=1;i<=n-1;i++)
    {
        if (get(a[i].u)!=get(a[i].v))
        {
            long long num=number[get(a[i].u)];                   
             ans+=(a[i].z+1)*(number[get(a[i].u)]*number[get(a[i].v)]-1);
            f[get(a[i].u)]=get(a[i].v);
            number[get(a[i].v)]+=num;
        }
    }
    printf("%lld\n",ans);
    }
    return 0;
}