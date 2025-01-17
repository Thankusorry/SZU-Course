#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

void Print(vector<vector<int>> A, int N)
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << setw(6) << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
int main()
{
    int N, M;
    cin >> N >> M;
    vector<vector<int>> A;
    A.resize(N + 1);
    for (int i = 0; i <= N; i++)
    {
        A[i].resize(N + 1, 88888);
    }
    for (int i = 0; i < M; i++)
    {
        int a, b,m;
        cin >> a >> b>>m;
        A[a][b]  = m;
    }
    for (int k = 1; k <= N; k++) // 第一层：节点
    {
        for (int i = 1; i <= N; i++) // 第二层 ：行
        {
            for (int j = 1; j <= N; j++) // 第三层 ：列
            {
                if (A[i][j] > (A[i][k] + A[k][j]))
                    A[i][j] = A[i][k] + A[k][j];
            }
        }
    }
    Print(A,N);

    return 0;
}