// 深度优先搜索实现Ford-Fulkerson算法的增广路径查找
// 参数:
//   u: 当前节点
//   t: 汇点
//   f: 当前路径上的最小剩余容量
//   visited: 访问标记数组
// 返回值: 找到的增广路径流量
int dfs(int u, int t, int f, bool *visited)
{
    // 如果到达汇点，返回当前路径上的最小剩余容量
    if (u == t)
    {
        return f;
    }
    visited[u] = true; // 标记当前节点已访问

    // 当前弧优化: 使用iter数组记录每个节点当前遍历到哪条边
    // 通过引用(&i)直接修改iter[u]的值，实现优化
    for (int &i = iter[u]; i != -1; i = edge_pool[i].next)
    {
        Edge &e = edge_pool[i]; // 获取当前边的引用

        // 如果目标节点未访问且边有剩余容量
        if (!visited[e.to] && e.capacity > 0)
        {
            // 递归搜索下一节点，传递当前路径的最小剩余容量
            int found_flow = dfs(e.to, t, std::min(f, e.capacity), visited);

            // 如果找到有效流量
            if (found_flow > 0)
            {
                // 更新正向边容量（减少）
                e.capacity -= found_flow;
                // 更新反向边容量（增加）
                edge_pool[e.rev].capacity += found_flow;
                return found_flow; // 返回找到的流量
            }
        }
    }
    return 0; // 没有找到增广路径
}

// Ford-Fulkerson算法主函数（指针版）
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
// 返回值: 最大流值
int fordFulkerson(int s, int t, int n)
{
    int max_flow = 0; // 最大流初始化为0

    // 动态分配访问标记数组（使用指针）
    bool *visited = new bool[n];

    // 不断寻找增广路径直到找不到为止
    while (true)
    {
        // 重置访问标记数组（全部设为false）
        std::fill(visited, visited + n, false);

        // 当前弧优化：每次搜索前重置iter数组为head数组
        // 这样每次DFS都从每个节点的第一条边开始
        for (int i = 0; i < n; ++i)
        {
            iter[i] = head[i];
        }

        // 从源点开始DFS，初始流量设为无穷大(INF)
        int flow = dfs(s, t, INF, visited);

        // 如果找不到增广路径，结束循环
        if (flow == 0)
        {
            break;
        }
        max_flow += flow; // 累加找到的流量
    }

    // 释放动态分配的visited数组
    delete[] visited;
    return max_flow; // 返回计算的最大流
}
