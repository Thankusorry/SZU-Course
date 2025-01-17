#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int n = 0, c = 0, k = 0, sum = 0;
    double x = 0, y = 0, r = 0;
    cin >> n;
    while (n--)
    {
        cin >> c >> k;
        cin >> x >> y >> r;
        for (int i = 0; i <= k; i++)
        {
            for (int j = 0; j <= c; j++)
            {
                if (sqrt(pow(x - j, 2) + pow((y - i), 2)) <= r)
                    sum++;
            }
        }
        cout << sum << endl;
    }
}