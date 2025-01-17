#include <iostream>
#include <string>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int max = -1;
        string A;
        cin >> A;
        for (int i = 1; i <= A.length() / 2;i++){
            for (int j = 0; j <= A.length() ;j++){
                string s = A.substr(j, i);//pos j len i --string
                if(A.find(s,j+i)!=string::npos){
                    max = i;
                    continue;
                }
            }
        }
        cout << max << endl;
    }
    return 0;
}