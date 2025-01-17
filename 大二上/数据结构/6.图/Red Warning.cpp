#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;
void print1(int i)
{
    cout << "City " << i << " is lost.\n";
}
void print2(int i)
{
    cout << "Red Alert: City " << i << " is lost!\n";
}

int find(int x, vector<int> p)
{
    if (x == p[x])
        return x;
    return p[x] = find(p[x], p);
}
void Union(int x, int y, vector<int> &p)
{
    int xx = find(x, p);
    int yy = find(y, p);
    if (xx != yy)
        p[xx] = yy;
}
int main()
{

    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    vector<int> p(n);
    for (int i = 0; i < n; i++)
        p[i] = i;
    vector<vector<int>> adj(n, vector<int>(n, 0x3f3f3f3f));
    for (int i = 0; i < m; i++)
    {
        int num1, num2;
        cin >> num1 >> num2;
        adj[num1][num2] = adj[num2][num1] = 1;
        Union(num1, num2, p);
    }
    int cnt = 0, cnt2 = 0;
    for (int i = 0; i < n; i++)
    {
        if (i == find(i, p))
            cnt++;
    }
    int K;
    cin >> K;
    while (K--)
    {
        int index;
        cin >> index;
        vector<vector<int>> adj2 = adj;
        cnt2 = 0;
        for (int i = 0; i < n; i++)
            p[i] = i;
        for (int i = 0; i < n; i++)
            adj2[index][i] = adj2[i][index] = 0x3f3f3f3f;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (adj2[i][j] != 0x3f3f3f3f)
                    Union(i, j, p);
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (i == find(i, p))
                cnt2++;
        }
        if (cnt2 - cnt > 1)
            print2(index);
        else
            print1(index);
    }
    cout << "Game Over.\n";
    return 0;
}