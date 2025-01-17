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

        int n;
        cin >> n;
        int m;
        cin >> m;
        vector<graph> A(n);
        map<string, int> M;
        for (int i = 0; i < n; i++)
        {
            cin >> A[i].data;
            // m.insert(pair<string, int>(A[i].data, i));
            M[A[i].data] = i;
        }
        for (int i = 0; i < m; i++)
        {
            string s1, s2;
            cin >> s1 >> s2;
            A[M[s1]].kids.push_back(M[s2]);
        }
        for (int i = 0; i < n; i++)
        {
            cout << i << " " << A[i].data;
            for (int j = 0; j < A[i].kids.size(); j++)
                cout << "-" << A[i].kids[j];
            cout << "-^\n";
        }
    }
}