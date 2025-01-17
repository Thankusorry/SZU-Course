#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
using namespace std;

struct graph
{
    string data;
    vector<int> kids;
};

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        char type;
        cin >> type;
        int n;
        cin >> n;
        map<string, int> M;
        vector<string> S(n);
        vector<int> d(n, 0), d1(n, 0), d2(n, 0);
        vector<vector<int>> adj(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            cin >> S[i];
            M[S[i]] = i;
        }
        int m;
        cin >> m;
        for (int i = 0; i < m; i++)
        {
            string s1, s2;
            cin >> s1 >> s2;
            if (type == 'U')
            {
                adj[M[s1]][M[s2]] = adj[M[s2]][M[s1]] = 1;
                d[M[s1]]++;
                d[M[s2]]++;
            }
            else
            {
                adj[M[s1]][M[s2]] = 1;
                d2[M[s1]]++;
                d1[M[s2]]++;
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (j)
                    cout << " ";
                cout << adj[i][j];
            }
            cout << endl;
        }
        for (int i = 0; i < n; i++)
        {
            cout << S[i];
            if (type == 'D')
            {
                if (d2[i] + d1[i])
                    cout << ": " << d2[i] << " " << d1[i] << " " << d1[i] + d2[i] << endl;
            }
            else
            {
                if (d[i])
                    cout << ": " << d[i];
                cout << endl;
            }
        }
    }
}