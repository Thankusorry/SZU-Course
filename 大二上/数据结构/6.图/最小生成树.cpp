#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

struct node
{
    string a, b;
    int dis;
    node(string a, string b, int c) : a(a), b(b), dis(c) {}
};
int select(int n, vector<int> &dis, vector<bool> &visited)
{
    int index = 0, min = 0x3f3f3f3f;
    for (int i = 0; i < n; i++)
    {
        if (visited[i] == false && dis[i] < min)
        {
            index = i;
            min = dis[i];
        }
    }
    return index;
}
void prim(vector<vector<int>> A, int v, vector<string> S)
{
    int n = A.size();
    vector<bool> visited(n, false);
    vector<int> dis(n, 0x3f3f3f3f);
    vector<int> adj(n);
    queue<node> q;
    long sum = 0;
    dis[v] = 0;
    for (int i = 0; i < n; i++)
    {
        int index = select(n, dis, visited);
        // 1.找不到了，结束
        if (dis[index] == 0x3f3f3f3f)
        {
            return;
        }
        sum += dis[index];
        visited[index] = true;
        if (index != v)
        {
            q.push(node(S[adj[index]], S[index], dis[index]));
        }
        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && dis[j] > A[index][j])
            {
                dis[j] = A[index][j];
                adj[j] = index;
            }
        }
    }
    cout << sum << endl
         << "prim:\n";
    while (!q.empty())
    {
        cout << q.front().a << " " << q.front().b << " " << q.front().dis << endl;
        q.pop();
    }
}

struct edge
{
    int v1, v2, weight;
    edge(int a, int b, int c) : v1(a), v2(b), weight(c) {}
    edge() {}
};
bool cmp(edge lhs, edge rhs)
{
    return lhs.weight < rhs.weight;
}

void kruskal(vector<edge> B, int v, vector<string> S, int n)
{
    cout << "kruskal:\n";
    vector<int> link(n);
    for (int i = 0; i < n; i++)
    {
        link[i] = i;
    }
    sort(B.begin(), B.end(), cmp);
    int edgeCount = 0;
    for (int i = 0; i < B.size(); i++)
    {
        int root1 = link[B[i].v1];
        int root2 = link[B[i].v2];
        if (root1 != root2)
        {
            if (B[i].v1 > B[i].v2)
                cout << S[B[i].v2] << " " << S[B[i].v1] << " " << B[i].weight << endl;
            else
                cout << S[B[i].v1] << " " << S[B[i].v2] << " " << B[i].weight << endl;
            for (int j = 0; j < n; j++)
            {
                if (link[j] == root2)
                {
                    link[j] = root1;
                }
            }
            edgeCount++;

            if (edgeCount == n - 1)
                break; // 生成树已完成
        }
    }
}
int main()
{
    int n;
    cin >> n;
    vector<string> S(n, "");
    for (int i = 0; i < n; i++)
        cin >> S[i];
    vector<vector<int>> A(n);
    for (int i = 0; i < n; i++)
    {
        A[i].resize(n, 0x3f3f3f3f);
        //        A[i].resize(n, 0);
    }
    int m;
    cin >> m;
    vector<edge> B(m);
    for (int i = 0; i < m; i++)
    {
        string a, b;
        int num;
        cin >> a >> b >> num;
        int index1, index2;
        for (int j = 0; j < n; j++)
        {
            if (S[j] == a)
                index1 = j;
            if (S[j] == b)
                index2 = j;
        }
        //        A[index1][index2] = num;
        A[index2][index1] = A[index1][index2] = num;
        B[i] = edge(index1, index2, num);
    }
    string ch;
    cin >> ch;
    int v;
    for (int j = 0; j < n; j++)
    {
        if (S[j] == ch)
            v = j;
    }
    prim(A, v, S);
    kruskal(B, v, S, n);
    return 0;
}