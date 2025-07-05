// Dinic算法 - BFS构建分层图
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
// 返回值: 如果汇点可达返回true，否则false
bool dinicBFS(int s, int t, int n)
{
    // 初始化所有节点的层级为-1（表示未访问）
    std::fill(level, level + n, -1);
    std::queue<int> q; // 使用STL队列进行BFS

    // 设置源点层级为0并入队
    level[s] = 0;
    q.push(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        // 遍历当前节点的所有出边
        for (int i = head[u]; i != -1; i = edge_pool[i].next)
        {
            Edge &e = edge_pool[i]; // 获取当前边的引用

            // 如果边有剩余容量且目标节点未访问
            if (e.capacity > 0 && level[e.to] < 0)
            {
                // 设置目标节点层级（当前节点层级+1）
                level[e.to] = level[u] + 1;
                q.push(e.to); // 目标节点入队
            }
        }
    }
    // 检查汇点是否可达（层级不为-1）
    return level[t] != -1;
}

// Dinic算法 - DFS搜索阻塞流
// 参数:
//   u: 当前节点
//   t: 汇点
//   f: 当前路径上的最小剩余容量
// 返回值: 从当前节点到汇点的最大流量
int dinicDFS(int u, int t, int f)
{
    // 如果到达汇点，返回当前路径上的最小剩余容量
    if (u == t)
        return f;

    int total_pushed = 0; // 记录从当前节点流出的总流量

    // 当前弧优化：通过引用(&i)直接修改iter[u]的值
    for (int &i = iter[u]; i != -1; i = edge_pool[i].next)
    {
        Edge &e = edge_pool[i]; // 获取当前边的引用

        // 检查是否满足分层图条件（层级递增）且边有剩余容量
        if (level[e.to] == level[u] + 1 && e.capacity > 0)
        {
            // 递归搜索下一节点，传递当前路径的最小剩余容量
            int pushed = dinicDFS(e.to, t, std::min(f, e.capacity));

            if (pushed > 0)
            {
                // 更新正向边容量（减少）
                e.capacity -= pushed;
                // 更新反向边容量（增加）
                edge_pool[e.rev].capacity += pushed;

                total_pushed += pushed; // 累加总流量
                f -= pushed;            // 减少当前路径的可用流量

                // 如果当前路径的流量已用完，提前退出
                if (f == 0)
                    break;
            }
        }
    }
    return total_pushed; // 返回从当前节点流出的总流量
}

// Dinic算法主函数
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
// 返回值: 最大流值
int dinic(int s, int t, int n)
{
    int max_flow = 0; // 最大流初始化为0

    // 只要还能构建出分层图（汇点可达）
    while (dinicBFS(s, t, n))
    {
        // 当前弧优化：重置每个节点的迭代器为邻接表头部
        for (int i = 0; i < n; i++)
        {
            iter[i] = head[i];
        }

        // 从源点开始DFS搜索阻塞流
        int flow_this_round = dinicDFS(s, t, INF);

        // 如果在分层图存在的情况下找不到流，结束算法
        if (flow_this_round == 0)
        {
            break;
        }

        // 累加本轮找到的流量
        max_flow += flow_this_round;
    }
    return max_flow; // 返回计算的最大流
}
