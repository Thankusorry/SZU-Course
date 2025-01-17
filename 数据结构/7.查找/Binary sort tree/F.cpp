#include<iostream>
#include<string>
#include<cstring>
using namespace std;
int main(){
    string s;
    int tap = 0;
    while(1){
        getline(cin,s);
        if(s == "-==-") break;
        for(unsigned int i = 0;i < s.length();i++){
            if(s[i] == 'y'){
                tap = 1;
                continue;
            }
            if(s[i] == 'o' && tap == 1){
                tap = 2;
                continue;
            }
            if(s[i] == 'u' && tap == 2){
                cout << "we";
                tap = 0;
                continue;
            } 
            if(tap==0)
                cout << s[i];
            if(tap == 1 && s[i] != 'o') cout << 'y' << s[i];
            else if(tap == 2 && s[i] != 'u') cout << "yo" << s[i];
            tap = 0;


        }
        cout << endl;
    }
    return 0;
}
