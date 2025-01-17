### F. 拯救007



### 题目描述



**主角在池心岛，需要直接踩着池子里一系列鳄鱼的大脑袋跳上岸去！**

设**鳄鱼池**是**长宽为100米的方形，中心坐标为 (0, 0)**，且东北角坐标为 (50, 50)。**池心岛是**以 **(0, 0) 为圆心、直径15米的圆**。给定池中分布的鳄鱼的坐标、以及00**7一次能跳跃的最大距离**，你需要告诉他是否有可能逃出生天。



### 输入

首先第一行给出两个正整数：**鳄鱼数量 *N***（≤100）和007一次能跳跃的**最大距离 *D***。随后 *N* 行，每行给出一条鳄鱼的 (*x*,*y*) 坐标**。注意：不会有两条鳄鱼待在同一个点上。**

**14 20**
25 -15
-25 28
8 49
29 15
-35 -2
5 28
27 -29
-8 -28
-20 -35
-25 -20
-13 29
-30 15
-35 40
12 12

### 输出

如果007有可能逃脱，就在一行中输出"Yes"，否则输出"No"。

Yes

+++

**Hint ：最短路径**

怎么应用呢？

最短路径是集合中不断加入点，更新状态，直至走完所有点，可以得到起始点到所有点最短路径。

鳄鱼看作不同的点

1.怎么确定起始点呢？

选择到圆心距离< D+r的点，多个点怎么处理？

2.怎么确定成功跳出呢？

到外面的距离 < D





为什么要N+2，而不行N+1然后每次判断

```cpp
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
```

