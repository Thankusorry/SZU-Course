#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <stack>
using namespace std;

int find(int x, vector<int> pre)
{
    while (pre[x] != -1)
        x = pre[x];
    return x;
}
void Union(int x, int y, vector<int> &p)
{
    int xx = find(x, p), yy = find(y, p);
    if (xx != yy)
        p[xx] = yy;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<int> pre(n, -1);
        vector<vector<int>> adj(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> adj[i][j];
                if (adj[i][j])
                    Union(i, j, pre);
            }
        }
        int cnt = 0;
        for (int i = 0; i < n; i++)
        {
            if (pre[i] == -1)
                cnt++;
        }
        if (cnt == 1)
            cout << "Yes\n";
        else
            cout << "No\n";
    }
}