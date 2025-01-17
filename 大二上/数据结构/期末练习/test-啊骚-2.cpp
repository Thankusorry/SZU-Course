#include <iostream>
#include <vector>
using namespace std;

#include <iostream>
#include <vector>
using namespace std;
int dis[] = {1, -1, 4, -4, 9, -9, 16, -16, 25, -25};
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int m;
        cin >> m;
        int n;
        cin >> n;
        vector<int> A(n);
        vector<int> Hash(m, -1);
        int cnt = 0;
        for (int i = 0; i < n; i++)
        {
            cnt = 0;
            cin >> A[i];
            int temp = A[i] % 11;
            int temp2 = temp;
            while (1)
            {
                if (Hash[temp2] == -1)
                {
                    Hash[temp2] = A[i];
                    break;
                }
                else
                {
                    temp2 = temp + dis[cnt++];
                    if (temp2 >= m)
                        temp2 =temp2%m;
                    else if(temp2<0)
                        temp2 += m;
                }
            }
        }
        for (int i = 0; i < m; i++)
        {
            if (i)
                cout << " ";
            if (Hash[i] != -1)
                cout << Hash[i];
            else
                cout << "NULL";
        }
        cout << endl;
        int k;
        cin >> k;
        while (k--)
        {
            int num;
            cin >> num;
            int cnt = 0;
            int temp = num % 11;
            int temp2 = temp;
            int index = 0;
            int flag = 0;
            while(1)
            {
                cnt++;
                if (Hash[temp2] == num)
                {
                    flag = 1;
                    break;
                }
                else if (Hash[temp2] == -1 || cnt==m)
                {
                    flag = 0;
                    break;
                }
                temp2=temp+dis[index++];
                if (temp2 >= m)
                    temp2 = temp2 % m;
                else if (temp2 < 0)
                    temp2 += m;
            }
            if (flag)
            {
                cout << "1 " << cnt << " " << temp2+1 << endl;
            }
            else
            {
                cout << "0 " << cnt << endl;
            }
        }
    }
    return 0;
}