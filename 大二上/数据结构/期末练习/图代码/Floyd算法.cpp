#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;
#define inf 0x3f3f3f3f

int main()
{
    int t;
    cin >> t;
    while (t--)
    {

        int n;
        cin >> n;
        vector<string> S(n);
        for (int i = 0; i < n; i++)
        {
            cin >> S[i];
        }
        vector<vector<int>> adj(n, vector<int>(n, inf));
        vector<vector<string>> path(n, vector<string>(n, ""));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                int temp;
                cin >> temp;
                if (temp)
                {
                    adj[i][j] = temp;
                    path[i][j] = S[i] + " " + S[j];
                }
            }
        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (adj[i][j] >= adj[i][k] + adj[k][j])
                    {
                        adj[i][j] = adj[i][k] + adj[k][j];
                        path[i][j] = path[i][k] + path[k][j].substr(S[k].size());
                    }
                }
            }
        }
        string v;
        cin >> v;
        int start;
        for (int i = 0; i < n; i++)
            if (S[i] == v)
                start = i;
        for (int i = 0; i < n; i++)
        {
            if (i == start)
                continue;
            cout << v << "-" << S[i] << "-";
            if (adj[start][i] == inf)
            {
                cout << "-1\n";
                continue;
            }
            cout << adj[start][i] << "----[";
            cout << path[start][i];
            cout << " ]\n";
        }
    }
}