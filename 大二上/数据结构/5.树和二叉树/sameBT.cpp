#include<iostream>

using namespace std;

bool issame(string s1,string s2,int i,int &j){
    //  cout << s1[i] << " " << s2[j] << endl;
    if(i>=s1.length() || j>=s2.length())
        return true;
        // 连续#处理
    if (s2[j]=='#' ){
        if(s1[i]=='#'){
            return true;
        }
        else
            return false;
    }
    if(s1[i]=='#')
    if(s1[i]!=s2[j])
        return false;
    bool left = issame(s1, s2, 2 * i + 1, ++j);
    bool right= issame(s1,s2,2*i+2,++j);

    return left && right;
}
int main(){
    int t;
    cin>>t;
    while(t--){
        string s1,s2;
        cin >> s1 >> s2;
        int i = 0, j = 0;
        if (issame(s1, s2, i, j))
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}