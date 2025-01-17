#include <iostream>
#include <vector>
#include <queue>
using namespace std;
struct point
{
    int x, y;
    point(int a, int b)
    {
        x = a;
        y = b;
    }
};
void bfs(vector<int> A[], int i, int j, int n, int m)
{ // 0  1->n n+1
    queue<point> q;
    point curr(i, j);
    int dir1[4] = {-1, 1, 0, 0}; // left right
    int dir2[4] = {0, 0, -1, 1}; // down up

    q.push(curr);
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        if (A[curr.x][curr.y] == 0)
        {
            A[curr.x][curr.y] = 1;
            for (int k = 0; k < 4; k++)
            {
                int a = curr.x + dir1[k];
                int b = curr.y + dir2[k];
                if (a >= 1 && b >= 1 && a <= n && b <= m && A[a][b] == 0)
                {
                    point curr2(a, b);
                    q.push(curr2);
                }
            }
        }
    }
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n, m;
        cin >> n >> m;
        vector<int> A[100];
        A[0].resize(m + 3);
        A[n + 1].resize(m + 3);
        for (int i = 1; i <= n; i++)
        {
            A[i].resize(m + 3);
            for (int j = 1; j <= m; j++)
            {
                cin >> A[i][j];
            }
        }
        for (int i = 0; i <= n + 1; i++)
        {
            A[i][0] = A[i][m + 1] = 0;
        }
        for (int i = 0; i <= m + 1; i++)
            A[0][i] = A[n + 1][i] = 0;

        int i1, j1;
        for (int i = 1; i <= m; i++)
        {
            bfs(A, 1, i, n, m); // 第一行
            bfs(A, n, i, n, m); // 第n行
        }
        for (int i = 1; i <= n; i++)
        {
            bfs(A, i, 1, n, m); // 第一列
            bfs(A, i, m, n, m); // 第m列
        }
        int cnt = 0;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                if (A[i][j] == 0)
                    cnt++;
            }
        }
        cout << cnt << endl;
    }
    return 0;
}