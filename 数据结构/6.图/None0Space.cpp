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
        int a, b;
        cin >> a >> b;
        A[a][b] = A[b][a] = 1;
    }
    // 计算最短距离
    for (int k = 1; k <= N; k++) // 第一层：节点
    {
        for (int i = 1; i <= N; i++) // 第二层 ：行
        {
            for (int j = 1; j <= N; j++) // 第三层 ：列
            {
                if (i == j)
                    continue;
                if (A[i][j] > (A[i][k] + A[k][j])) // 所有i节点通过k节点，再到j节点距离 求最小
                    A[i][j] = A[i][k] + A[k][j];
            }
        }
    }
    Print(A, N);
    int K;
    cin >> K;
    int B[K];
    for (int i = 0; i < K; i++)
    {
        cin >> B[i];
        double sum = 0;
        for (int j = 1; j <= N; j++)
        {
            if (j != B[i])
                sum += A[B[i]][j];
        }                                                              
        // 取巧：实际上不为零，不联通的话设置分母足够大，近似为零
        cout << "Cc(" << B[i] << ")=" << fixed << setprecision(2) << (N - 1) / sum << endl;
    }
    return 0;
}