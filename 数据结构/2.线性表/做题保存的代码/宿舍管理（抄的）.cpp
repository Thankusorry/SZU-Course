#include <iostream>
#include <list>
using namespace std;
struct room{
    string Name;
    int Number;
};
typedef list<room> LISTINT;

bool operator< (room &a, room& b){
    if (a.Number < b.Number) return 1;
    return 0;
}


int main() {
    LISTINT correct;
    LISTINT error;
    int t;
    cin>>t;
    int a[1000],b[1000]={0};
    for(int i=0;i<t;i++) {
        string Name;
        int Number;
        cin>>Name>>Number;
        a[i]=Number;
        correct.push_back({Name,Number});
    }
    for(int i=101;i<121;i++) {
        int k=0;
        for (int j=0;j<t;j++) {
            if(a[j]==i) 
                k=1;
        }
        if(!k)
            error.push_back({"",i});
    }
    int m;
    cin>>m;
    while(m--){
        string Signal;
        cin>>Signal;
        if(Signal=="assign"){
            string Name;
            cin>>Name;
            int Number2=error.front().Number;
            error.pop_front();
            correct.push_front({Name,Number2});
        }
        else if(Signal=="return"){
            int Number3;
            cin>>Number3;
            for(auto i=correct.begin();i!=correct.end();i++){
                if(i->Number==Number3){
                    correct.erase(i);
                    break;
                }
            }
            error.push_back({"",Number3});
        }
        else if(Signal=="display_used"){
            LISTINT::iterator j;
            correct.sort();
            for(auto j=correct.begin(); j!=correct.end();j++) {
                if(j!=correct.begin()){
                    cout<<"-";
                }
                cout<<j->Name<<"("<<j->Number<<")";
            }
            cout<<endl;
        }
        else if(Signal=="display_free"){
            for(auto q=error.begin();q!=error.end();q++) {
                if(q!=error.begin()){
                    cout<<"-";
                }
                cout<<q->Number;
            }
            cout<<endl;
        }
 
 
    }
 
}