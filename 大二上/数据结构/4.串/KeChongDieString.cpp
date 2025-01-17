#include <iostream>
#include <string>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string P;
        int N;
        cin >> P >> N;
        for (int i = 0; i < N;i++){
            string test;
            cin >> test;
            cout << test << ":";
            int cnt = 0;
            for (int j = 0; j < P.length();j++){
                if(P.find(test,j)!=string::npos){
                    cnt++;
                    j = P.find(test, j);
                }
            }
            cout << cnt << endl;
        }
    }
    return 0;
}