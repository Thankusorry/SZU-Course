#include <iostream>
#include <vector>
using namespace std;

struct node{
    char ch;
    int index;
};
int main(){
    int n;
    cin >> n;
    vector<node> v;
    v.resize(n);
    for (int i = 0; i < n;i++){
        cin >> v[i].ch;
    }
    for(int i=0;i<n;i++)
        cin >> v[i].index;
    for (int i = 0;i<n;i++){
        cout << v[i].ch << " ";
        int flag = 1;
        for (int j = 0;j<n;j++){
            if(v[j].index==i ){
                cout << j << " ";
                flag = 0;
            }
        }
            if(flag)
                cout << "-1 ";
        cout << endl;
    }
} 