/* #include <iostream>
#include <vector>
using namespace std;

struct arcnode
{
    int adj;
    int w;
};
vector<int> getindegree(int n, vector<arcnode> g[]) vector<int> etime[]
{
    vector<int> in(n, 0);
    for (int i = 0; i < n; i++)
    {
        if (!in[i])
        {
            q.push(i);
        }
    }
}
void getlater(int n, vector<arcnode> g[], vector<int>)
{
    int last = *(list.end() - 1);
    ltime[last] = etime[last];
    // 更新入度邻接点最晚 邻接矩阵：访问列 邻接表：遍历所有节点入度是否等于顶点
    for (i = list.size() - 1; i > 0; i--)
    {
        ltime[adj] = min{ltime[adj], ltime[list[i] - x.w]};
        // 邻接表
        for (j = 0; j < n; j++)
        {
            for (auto x : g[j])
            {
                if (x.adj == list[i])
                {
                    ltime[j] = min{ltime[j], ltime[list[i] - x.w]};
                }
            }
        }
    }
}
while (!q.empty())
{
    int front = q.front();
    q.pop();
    topolist.push_back(front);
    for (auto x : g[front])
    {
        in[x.adj]--;
        if (!in[x.adj])
        {
            q.push(x.adj);
        }
        
        etime[x.adj]=max(etime[x.adj],etime[front]+x.w)
         
    }
}
int main()
{
    vector<arcnode> g[N];
    cin >> n >> e;
    while (e--)
    {
        cin >> v1 >> v2 >> w;
    }
} */