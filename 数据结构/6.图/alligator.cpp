#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

struct Point
{
    int x, y;
};

bool Jump(int x1, int y1, int x2, int y2, int D)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) <= D;
}

bool Reach(int x, int y, int D)
{
    return (D + abs(x) >= 50) || (D + abs(y) >= 50);
}
int select(int n,vector<bool> final, vector<int> D)
{
    int index = 0, min = 0x3f3f3f3f;
    for (int i = 0; i < n + 2;i++){
        if(!final[i] &&D[i]<min ){
            index = i;
            min = D[i];
        }
    }
    return index;
}
void dijkstra(int n, vector<vector<int>> adj, int start, vector<Point> P, int step)
{
    vector<int> D(n + 2, 0x3f3f3f3f);
    vector<bool> final(n + 2, false);
    D[start] = 0;
    final[start] = true;
    for (int i = 0; i < n+2;i++){
        int index = select(n, final, D);
        if(D[index]==0x3f3f3f3f) {
            break;
        }
        final[index] = true;
        if(index==n+1){
            cout<<"Yes\n";
            return;
        }
        for(int j=0;j<n+2;j++){
            if (!final[j] && D[j]>D[index]+adj[index][j]){
                D[j] = D[index] + adj[index][j];
            }
        }
        
    }
    cout << "No\n";
}
int main()
{
    int N, D;
    cin >> N >> D;
    vector<Point> P(N+2);
    for (int i = 0; i < N; i++)
    {
        cin >> P[i].x >> P[i].y;
    }
    vector<vector<int>> adj(N + 2, vector<int>(N + 2, 0x3f3f3f3f));
    // 确定起点 池心岛与鳄鱼节点相连
    for (int i = 0; i < N; i++)
    {
        if (sqrt(pow(P[i].x, 2) + pow(P[i].y, 2)) <= D + 7.5)
        {
            adj[0][i + 1] = 1;
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (Jump(P[i].x, P[i].y, P[j].x, P[j].y, D))
            {
                adj[i + 1][j + 1] = adj[j + 1][i + 1] = 1;
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (Reach(P[i].x, P[i].y, D))
        {
            adj[i + 1][N + 1] = 1; // 鳄鱼到岸边
        }
    }
    dijkstra(N, adj, 0, P, D);
}