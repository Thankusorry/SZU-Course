#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<int> A(n + 1, 0);
    for (int i = 1; i <= n; i++)
        cin >> A[i];
    int k;
    cin >> k;
    // k块
    vector<int> B(k, 0), index(k+1, 0);
    for (int i = 0; i < k; i++)
        cin >> B[i];
    index[0] = 1,index[k]=n+1;
    for (int i = 1; i <= n; i++)
    {
        if (A[i] > B[0] && index[1] == 0)
        {
            index[1] = i ;
        }
        if (A[i] > B[1])
        {
            index[2] = i ;
            break;
        }
    }
    int t;
    cin >> t;
    while (t--)
    {
        int num;
        cin >> num;
        int start = num <= B[0] ? 0 : num <= B[1] ? 1
                                 : num <= B[2]   ? 2
                                                : -1;
        int cnt = start+1;
        int flag = 0;
        int i;
        for (i = index[start]; i < index[start + 1]; i++)
        {
            if (A[i] == num)
            {
                cnt++;
                flag = 1;
                break;
            }
            cnt++;
        }
        if (!flag)
            cout << "error\n";
        else
            cout << i << "-" << cnt << endl;
    }
}