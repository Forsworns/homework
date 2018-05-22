#include<iostream>
using namespace std;
char roadType[100000];
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int time,roads,up,flat,down,tempTimeSum=0;
    cin>>time>>roads>>up>>flat>>down;
    for(int i=0;i<roads;++i){
        cin>>roadType[i];
    }
    for(int i=0;i<roads;++i){
        if(roadType[i]=='f'){
            tempTimeSum+=(2*flat); //往返一起考虑
        }else {
            tempTimeSum+=(down+up);
        }
        if(tempTimeSum>time){
            cout<<i;
            return 0;
        }
    }
    cout<<roads;
    return 0;
}