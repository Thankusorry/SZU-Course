### E. 道路建设 (Ver. I)



#### 题目描述



有N个村庄，编号从1到N，你应该建造一些道路，使每个村庄都可以相互连接。

两个村A和B是相连的，当且仅当A和B之间有一条道路，或者存在一个村C使得在A和C之间有一条道路，并且C和B相连。

现在一些村庄之间已经有一些道路，你的任务就是修建一些道路，使所有村庄都连通起来，并且所有道路的长度总和是最小的。



#### 输入



测试数据有多组

第一行是整数N（3 <= N <= 100），代表村庄的数量。 然后是N行，其中第i行包含N个整数，这些N个整数中的第j个是村庄i和村庄j之间的距离（距离是[1,1000]内的整数）。

然后是整数Q（0 <= Q <= N *（N + 1）/ 2），接下来是Q行，每行包含两个整数a和b（1 <= a <b <= N），代表着村庄a和村庄b之间的道路已经建成。

3
0 990 692
990 0 179
692 179 0
1
1 2

#### 输出



对于每组测试数据

输出一个整数，表示要构建的道路的长度总和最小值

179

+++

思路：

选 路 （选边） ——总花费最小 （总权值最小） ——最小生成树—— prim算法 or 考入kruskal 算法

**不需要找路径，所以adj可以除去**
**记得是双向边，对称赋值**

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

int select(vector<int> D, vector<int> visited)
{
    int index = 0, min = 0x3f3f3f3f;
    for (int i = 0; i < D.size(); i++)
    {
        if (visited[i] == 0 && D[i] < min)
        {
            index = i;
            min = D[i];
        }
    }
    return index;
}
void prim(vector<vector<int>> dis)
{
    int n = dis.size();
    vector<int> visited(n, 0);
    vector<int> D(n, 0x3f3f3f3f);
    int sum = 0;
    int index = 0;
    D[index] = 0;
    visited[index] = 1;
    for (int i = 0; i < n; i++)
    {
        int index = select(D, visited);
        visited[index] = 1;
        if (D[index] == 0x3f3f3f3f)
        {
            continue;
        }
        sum += D[index];
        for (int j = 0; j < n; j++)
        {
            if (visited[j] == 0 && D[j] > dis[index][j])
            {
                D[j] = dis[index][j];
            }
        }
    }
    cout << sum << endl;
}
int main()
{
    int n;
    while (cin >> n)
    {

        vector<vector<int>> dis(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> dis[i][j];
            }
        }
        int Q;
        cin >> Q;
        for (int i = 0; i < Q; i++)
        {
            int a, b;
            cin >> a >> b;
            dis[a - 1][b - 1] = dis[b - 1][a - 1] = 0;
        }

        prim(dis);
    }
    return 0;
}

```

