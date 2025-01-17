#include <iostream>

using namespace std;
void Tuopu(int A[][100], int n)
{
    int in[n] = {0};
    int visited[n] = {0};
    int num = 0;
    for (int i = 0; i < n;i++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (A[j][i])
                    in[i]++;
            }
        }
        int v = -1;
        for (int i = 0; i < n; i++)
        {
            if (!in[i])
            {
                v = i;
                break;
            }
        }
        if(v==-1)
            break;
        for (int i = 0; i < n; i++)
            A[v][i] = 0;
        visited[v] = 1;
        cout << v << " ";
    }
    cout << endl;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        int A[n][n];
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> A[i][j];
            }
        }
        Tuopu(A, n);
    }
}