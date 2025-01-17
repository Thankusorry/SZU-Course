#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
using namespace std;

void dfs(vector<vector<int>> adj, int n, int i, vector<bool> &visited)
{

    visited[i] = true;
    cout << i << " ";
    for (int j = 0; j < n; j++)
    {
        if (j == i)
            continue;
        if (adj[i][j] && !visited[j])
        {
            dfs(adj, n, j, visited);
        }
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
        vector<bool> visited(n, false);
        vector<vector<int>> adj(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                cin >> adj[i][j];
        }
        for (int i = 0; i < n; i++)
        {
            if (!visited[i])
                dfs(adj, n, i, visited);
        }
        cout << endl;
    }
}