#include <iostream>
#include <vector>
#include<iomanip>
#include<cmath>
using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int num;
        cin >> num;
        int cnt = 1;
        double left=0, right=num;
        double x = (left + right) / 2;
        double x2 = pow(x, 2);
        while(abs(x2-num)>=0.00001){
            if(x2>num){
                right = x;
            }
            if(x2<num){
                left = x;
            }
            x = (right + left) / 2;
            x2 = pow(x, 2);
            cnt++;
        }
        cout << cnt <<" "<<fixed<<setprecision(3)<<x<< endl;
    }
}