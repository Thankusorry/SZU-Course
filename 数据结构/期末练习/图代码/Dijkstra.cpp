#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;
#define inf 0x3f3f3f3f
void Dijkstra(vector<string> S, vector<vector<int>> adj, int n, int v)
{
    vector<bool> vis(n, false);
    vector<int> dis(n, inf);
    vector<int> pre(n,-1);
    dis[v] = 0;
    vis[v] = true;
    for (int i = 0; i < n; i++)
    {
        if (dis[i] > adj[v][i])
        {
            dis[i] = adj[v][i];
            pre[i] = v;
        }
    }
    for (int i = 0; i < n - 1; i++)
    {
        int index = 0, mini = inf;
        for (int j = 0; j < n; j++)
        {
            if (!vis[j] && dis[j] > mini)
            {
                index = j;
                mini = dis[j];
            }
        }

        if (mini == inf)
            break;
        vis[index] = true;
        dis[index] = mini;
        for (int k = 0; k < n; k++)
        {
            if (dis[k] > adj[index][k])
            {
                dis[k] = adj[index][k];
                pre[k] = index;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (i == v)
            continue;
        cout << S[v] << "-" << S[i] << "-";
        if (dis[i] == inf)
        {
            cout << "-1\n";
            continue;
        }
        cout << dis[i] << "----[";
        string s;
        while(pre[i]!=-1){
            s=" "+S[i]+s;
            i = pre[i];
        }
        cout << s<<" ]\n";
    }
}
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
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                int temp;
                cin >> temp;
                if (temp)
                {
                    adj[i][j] = temp;
                }
            }
        string v;
        cin >> v;
        int start;
        for (int i = 0; i < n; i++)
            if (S[i] == v)
                start = i;
        Dijkstra(S, adj, n, start);


    }
}