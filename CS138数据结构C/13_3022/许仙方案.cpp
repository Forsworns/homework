#include<iostream>
#include <string>
using namespace std;
int m;

string invert(string &src)
{
    string newStr=src;
    for(int i=src.length()-1,j=0; i>=0; --i,++j)
        newStr[j]=src[i];

    return newStr;
}

string intAdd(string &rs1,string &rs2)
{
    string str1=invert(rs1);
    string str2=invert(rs2);

    if(str1.length()<str2.length())
        str1.swap(str2);

    for(size_t i=0; i!=str2.length(); ++i)
    {
        char c1=str1[i];
        char c2=str2[i];
        int t=((int)c1-48)+((int)c2-48);
        if(t>=10)
        {
            //½øÎ»
            int x=t/10;
            t%=10;
            size_t n=i+1;
            do
            {
                int t1=(int)str1[n]-48+x;
                if(t1>=10)
                {
                    str1[n]=(char)(t1%10+48);
                    ++n;
                }
                else
                {
                    str1[n]=(char)(t1+48);
                }
                if(n==str1.length())
                {
                    str1+="1";
                    break;
                }
                x=t1/10;
            }
            while(x!=0);

            str1[i]=(char)(t+48);
        }
        else
        {
            str1[i]=(char)(t+48);
        }
    }
    string &rstrResult=str1;
    string strOut=invert(rstrResult);
    return strOut;
}

string f(int n){
    n++;
    int i;
    string table[n];

    table[1]="1";
    table[2]="2";
    table[3]="4";
    string r;

    for(i=4;i<n;i++){
        r=intAdd(table[i-1],table[i-2]);
        table[i]=intAdd(r,table[i-3]);
    }
    return table[n-1];
}

int main(){
	cin>>m;
    cout <<f(m+1);
    return 0;
}
