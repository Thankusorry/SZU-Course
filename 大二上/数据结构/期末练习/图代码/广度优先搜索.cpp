#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <stack>
using namespace std;

void bfs(vector<vector<int>> adj, int n, int i, vector<bool> &visited)
{
    queue<int> q;
    q.push(i);
    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        if (!visited[curr])
        {
            cout << curr << " ";
            visited[curr] = true;
        }
        for (int j = 0; j < n; j++)
        {
            if (adj[curr][j] && !visited[j])
                q.push(j);
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
                bfs(adj, n, i, visited);
        }

        cout << endl;
    }
}