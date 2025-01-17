#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

int select(int n, vector<int> D, vector<int> final)
{
    int index = 0, mindis = 0x3f3f3f3f;
    for (int i = 0; i < n; i++)
    {
        if (!final[i] && mindis > D[i])
        {
            index = i;
            mindis = D[i];
        }
    }
    return index;
}
void dij(int n, vector<vector<int>> adj, int index, vector<string> S)
{
    vector<int> D(n, 0x3f3f3f3f);
    vector<int> final(n, 0);
    vector<int> path(n);
    vector<string> temp(n);

    D[index] = 0;
    for (int i = 0; i < n; i++)
    {
        int index = select(n, D, final);
        if (D[index] == 0x3f3f3f3f)
        {
            D[index] = -1;
            final[index] = -1;
            continue;
        }
        final[index] = 1;
        for (int j = 0; j < n; j++)
        {
            if (!final[j] && D[index] + adj[index][j] < D[j])
            {
                D[j] = D[index] + adj[index][j];
                path[j] = index;
                // temp[j] += " " + S[j];
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (i != index)
            cout << S[index] << "-" << S[i] << "-" << D[i];
        if (D[i] != -1)
        {
            cout << "----[" << S[index];
            int j = i;
            string temp = "";
            while (j != index)
            {
                temp = " " + S[j] + temp;
                j = path[index];
            }
            cout << temp << " ]\n";
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
        vector<string> S(n);
        for (int i = 0; i < n; i++)
            cin >> S[i];
        vector<vector<int> > adj(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> adj[i][j];
            }
        }
        int v;
        cin >> v;
//        int index = find(S.begin(), S.end(), v) - S.begin();
        dij(n, adj, index, S);
    }

    return 0;
}