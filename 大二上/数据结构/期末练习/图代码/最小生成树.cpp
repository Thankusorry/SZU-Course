#include <iostream>
#include <queue>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
#define INF 0x3f3f3f3f
struct edge
{
    int v1, v2;
    int w;
    edge() {}
    edge(int a, int b, int w) : v1(a), v2(b), w(w) {}
};
int sum = 0;
void prim(vector<string> S, vector<vector<int>> adj, int n, int v)
{
    vector<int> vis(n, -1);
    vector<int> dis(n, INF);
    vector<int> pre(n, -1);
    vis[v] = 1;
    dis[v] = 0;
    queue<edge> q;
    for (int k = 0; k < n; k++)
    {
        if (vis[k] < 0 && dis[k] > adj[v][k])
        {
            dis[k] = adj[v][k];
            pre[k] = v;
        }
    }
    for (int i = 1; i < n; i++)
    {
        int index = 0, min1 = INF;
        for (int j = 0; j < n; j++)
        {
            if (vis[j] < 0 && dis[j] < min1)
            {
                min1 = dis[j];
                index = j;
            }
        }
        if (dis[index] == INF)
        {
            break;
        }
        vis[index] = 1;
        dis[index] = min1;
        sum += min1;
        q.push(edge(pre[index], index, min1));
        for (int k = 0; k < n; k++)
        {
            if (vis[k] < 0)
                if (dis[k] > adj[index][k])
                {
                    dis[k] = adj[index][k];
                    pre[k] = index;
                }
        }
    }
    cout << sum << endl
         << "prim:\n";
    while (!q.empty())
    {
        edge z = q.front();
        cout << S[z.v1] << " " << S[z.v2] << " " << z.w << endl;
        ;
        q.pop();
    }
}

bool cmp(edge a, edge b)
{
    return a.w < b.w;
}
int find(int x, vector<int> pre)
{
    while (pre[x] != -1)
        x = pre[x];
    return x;
}
void Union(int x, int y, vector<int> &pre)
{
    int xx = find(x, pre), yy = find(y, pre);
    if (xx != yy)
        pre[xx] = yy;
}
void kruskal(vector<string> S, vector<edge> &E, int n, vector<int> pre)
{
    sort(E.begin(), E.end(), cmp);
    int m = E.size();
    int index = 0;
    for (int i = 0; i < m; i++)
    {
        int v1 = E[i].v1, v2 = E[i].v2;
        if (find(v1, pre) != find(v2, pre) || (pre[v1] == -1 && pre[v2] == -1))
        {
            if (v1 < v2)
                cout << S[E[i].v1] << " " << S[E[i].v2] << " " << E[i].w << endl;
            else
                cout << S[E[i].v2] << " " << S[E[i].v1] << " " << E[i].w << endl;
            Union(v1, v2, pre);
        }
    }
}
int main()
{
    int n;
    cin >> n;
    vector<string> S(n);
    vector<vector<int>> adj(n, vector<int>(n, INF));
    map<string, int> M;
    for (int i = 0; i < n; i++)
    {
        cin >> S[i];
        M[S[i]] = i;
    }
    int m;
    cin >> m;
    vector<edge> E(m);
    vector<int> pre(n, -1);
    for (int i = 0; i < m; i++)
    {
        string s1, s2;
        int w;
        cin >> s1 >> s2 >> w;
        E[i] = edge(M[s1], M[s2], w);
        adj[M[s1]][M[s2]] = adj[M[s2]][M[s1]] = w;
    }
    string v;
    cin >> v;

    prim(S, adj, n, M[v]);
    cout << "kruskal:\n";
    kruskal(S, E, n, pre);
    return 0;
}