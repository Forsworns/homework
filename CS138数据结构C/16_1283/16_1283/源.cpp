#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
int prenode[1050]; 
int num[350]={0};
int len = 1;
//2�ε�power��
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
 
//��
int find(int x){//���ҵ�x���ڵ���ͼ�ĸ��ڵ� 
    int res = x;
    while(res != prenode[res] ){//res�Ǹ��ڵ�������� res�ĸ��ڵ��Ǳ���
        res = prenode[res];
    }
    //ѹ��·�� �ô˴β�ѯ�е������漰�Ľڵ���ϼ��ڵ㶼�Ǹ��ڵ� �Ӷ������γ�ֻ�ж��������ṹ(���ܱ�֤һ���� ������ı�˳���й�)
    while(x != res){
        int pre = prenode[x];
        prenode[x] = res;
        x = pre;
    }
    return res;
}
//��
void join(int x, int y){//�� x �� y ����ͬһ��ͼ��
    int rootx = find(x) , rooty = find(y);
    if(rootx != rooty)//x,y������ͬһ��ͼʱ���в��ı�Ҫ
        prenode[rootx] = rooty;//����ͬһ��ͼ
    return;
}
 
 
int main(int argc, char const *argv[])
{
    
    int n,m; 
    cin>>n>>m;
    //��ʼ��n���� ���ڵ�Ϊ���� 
    for (int i = 1; i <= n; ++i)
        prenode[i] = i; 
    //����ÿ����
    for (int i = 0; i < m; ++i)
    {
        int x,y;
        cin>>x>>y;
        join(x,y);   
    }
    //����ͨ����ĸ���k 
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