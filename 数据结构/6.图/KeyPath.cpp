#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
int max(int a, int b)
{
    return a > b ? a : b;
}
int min(int a, int b)
{
    return a < b ? a : b;
}

int main()
{
    int n;
    cin >> n;
    vector<vector<int>> adj(n, vector<int>(n, 0));
    vector<int> vetime(n, 0), vltime(n, 0x3f3f3f3f), ind(n, 0);
    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int a, b, w;
        cin >> a >> b >> w;
        // A[a].adj = b;
        adj[a][b] = w;
        ind[b]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++)
    {
        if (!ind[i])
        {
            q.push(i);
            ind[i] = -1;
        }
    }

    vector<int> tp;
    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        tp.push_back(curr);
        for (int i = 0; i < n; i++)
        {
            if (adj[curr][i])
                ind[i]--;
            if (!ind[i])
            {
                q.push(i);
                ind[i] = -1; // 不能省
            }
        }
    }
    // cout << "tp\n";
    // for (int i = 0; i < n; i++) {
    //     cout << tp[i] << " ";
    // }
    // cout << endl;
    int start = tp[0];
    vetime[start] = 0;
    for (int i = 1; i < tp.size(); i++)
    {
        int v = tp[i];
        int Max = 0;
        for (int j = 0; j < n; j++)
        {
            if (adj[j][v])
                Max = max(Max, adj[j][v] + vetime[j]);
        }
        vetime[v] = Max;
    }

    int end = tp[n - 1];
    vltime[end] = vetime[end];
    for (int i = n - 2; i >= 0; i--)
    {
        int v = tp[i];
        int Min = 0x3f3f3f3f;
        for (int j = 0; j < n; j++)
        {
            if (adj[v][j])
            { // v=3 <3,4,15> 3=vl[4]-15
                Min = min(Min, vltime[j] - adj[v][j]);
            }
        }
        vltime[v] = Min;
    }

    for (int i = 0; i < n; i++)
    {
        cout << vetime[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        cout << vltime[i] << " ";
    }
}