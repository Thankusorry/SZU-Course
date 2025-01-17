#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
using namespace std;

int main()
{
    int N;
    cin >> N;
    vector<int> H(N), H2(N), hash;
    for (int i = 0; i < N; i++)
        cin >> H[i];

    H2 = H;
    sort(H2.begin(), H2.end()); // 排序
                                //-1 1 12 13 21 22 27
    for (int t = 0; t < N; t++)
    {
        for (int i = 0; i < N; i++)
        {
            if (H2[i] == -1)
                continue;
            int num = H2[i], temp = num % N;
            if (num == H[temp])
            {
                hash.push_back(num);
                H[temp] = H2[i] = -1;
                break;
            }
            else
            {
                for (int k = temp;; k++)
                {
                    if (k == N)
                        k = 0;
                    if (H[k] == num)
                    {
                        hash.push_back(num);
                        H[k]=H2[i]=-1;
                        break;
                    }
                    if (H[k] != -1)
                        break;
                }
            }

           
        }
    }
    for (int i = 0; i < hash.size(); i++)
    {
        if (i != 0)
            cout << " ";
        cout << hash[i];
    }
    return 0;
}