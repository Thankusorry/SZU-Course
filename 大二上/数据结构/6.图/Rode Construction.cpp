#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

int select(vector<int> D, vector<int> visited)
{
    int index = 0, min = 0x3f3f3f3f;
    for (int i = 0; i < D.size(); i++)
    {
        if (visited[i] == 0 && D[i] < min)
        {
            index = i;
            min = D[i];
        }
    }
    return index;
}
void prim(vector<vector<int>> dis)
{
    int n = dis.size();
    vector<int> visited(n, 0);
    vector<int> D(n, 0x3f3f3f3f);
    int sum = 0;
    int index = 0;
    D[index] = 0;
    visited[index] = 1;
    for (int i = 0; i < n; i++)
    {
        int index = select(D, visited);
        visited[index] = 1;
        if (D[index] == 0x3f3f3f3f)
        {
            continue;
        }
        sum += D[index];
        for (int j = 0; j < n; j++)
        {
            if (visited[j] == 0 && D[j] > dis[index][j])
            {
                D[j] = dis[index][j];
            }
        }
    }
    cout << sum << endl;
}
int main()
{
    int n;
    while (cin >> n)
    {

        vector<vector<int>> dis(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> dis[i][j];
            }
        }
        int Q;
        cin >> Q;
        for (int i = 0; i < Q; i++)
        {
            int a, b;
            cin >> a >> b;
            dis[a - 1][b - 1] = dis[b - 1][a - 1] = 0;
        }

        prim(dis);
    }
    return 0;
}
