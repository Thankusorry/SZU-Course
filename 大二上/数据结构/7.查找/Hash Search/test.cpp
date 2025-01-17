#include <iostream>
using namespace std;
int ka(int n){
    while(n>0){
        if (n % 2 == 0)
            n = n / 2;
        else
            n = (3 * n + 1) / 2;
        if(n==1)
            return 1;
    }
    return 0;
}
int main()
{
    int n, count = 0;
    cin >> n;
    for(int i=1;i<=1024;i++)
    {
        if(!ka(i))
            cout << "i" << endl;

        
    }
}