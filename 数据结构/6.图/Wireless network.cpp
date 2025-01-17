#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#define INF 0x3f3f3f3f
struct point
{
    int x, y;
    point() {}
    point(int a, int b) : x(a), y(b) {}
};
double getdis(point a, point b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
void collect(vector<point> &p, vector<vector<int>> &adj, vector<int> &OK, int d, int N)
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = i + 1; j <= N; j++)
        {
            if (OK[i] && OK[j] && getdis(p[i], p[j]) <= d)
            {
                adj[i][j] = adj[j][i] = 1;
            }
        }
    }
    for (int k = 1; k <= N; k++)
    {
        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                adj[i][j] = max(adj[i][j], adj[i][k] + adj[k][j]);
            }
        }
    }
}

int main()
{
    int N;
    double d;
    cin >> N >> d;
    vector<point> p(N);
    for (int i = 1; i <= N; i++)
    {
        int x, y;
        cin >> x >> y;
        p[i] = point(x, y);
    }
    vector<int> OK(N + 1, 0);
    vector<vector<int>> adj(N + 1, vector<int>(N + 1, INF));
    char ch;
    int cnt = 0;
    while (cin >> ch)
    {
        if (ch == 'O')
        {
            int num;
            cin >> num;
            OK[num] = 1;
        }
        else if (ch == 'S')
        {
            if (!cnt)
            {
                cnt++;
                collect(p, adj, OK, d, N);
            }
            int a, b;
            cin >> a >> b;
            if (adj[a][b] != INF &&OK[a] &&OK[b])
                cout << "SUCCESS\n";
            else
                cout << "FAIL\n";
        }
    }
}