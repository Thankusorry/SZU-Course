// 使用BFS在残留图中寻找从源点s到汇点t的增广路径
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
//   parent_node: 存储每个节点的父节点（用于回溯路径）
//   parent_edge: 存储到达每个节点的边索引（关键：用于更新残留图）
//   visited: 访问标记数组
// 返回值: 如果找到增广路径返回true，否则false
bool bfs_adj(int s, int t, int n, int *parent_node, int *parent_edge, bool *visited)
{
    // 使用数组模拟队列（避免STL开销）
    int *queue = new int[n];
    int head1 = 0, tail = 0; // 队列头尾指针

    // 初始化源点
    visited[s] = true;
    parent_node[s] = -1; // 源点没有父节点
    queue[tail++] = s;   // 源点入队

    // BFS遍历
    while (head1 < tail)
    {
        int u = queue[head1++]; // 出队一个节点

        // 遍历当前节点的所有出边
        for (int i = head[u]; i != -1; i = edge_pool[i].next)
        {
            Edge &e = edge_pool[i]; // 获取当前边的引用

            // 如果目标节点未访问且边有剩余容量
            if (!visited[e.to] && e.capacity > 0)
            {
                visited[e.to] = true;  // 标记已访问
                parent_node[e.to] = u; // 记录父节点（用于回溯路径）
                parent_edge[e.to] = i; // 【关键】记录到达该节点的边索引
                queue[tail++] = e.to;  // 目标节点入队

                // 提前退出优化：找到汇点即可返回
                if (e.to == t)
                {
                    delete[] queue; // 释放队列内存
                    return true;    // 找到增广路径
                }
            }
        }
    }

    delete[] queue; // 释放队列内存
    return false;   // 无法到达汇点
}

// Edmonds-Karp算法主函数（邻接表版）
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
// 返回值: 最大流值
int EK_adj(int s, int t, int n)
{
    int max_flow = 0; // 最大流初始化为0

    // 分配辅助数组（整个算法生命周期只需一次分配）
    int *parent_node = new int[n]; // 存储每个节点的父节点
    int *parent_edge = new int[n]; // 存储到达每个节点的边索引
    bool *visited = new bool[n];   // 访问标记数组

    // 主循环：不断寻找增广路径直到找不到为止
    while (true)
    {
        // 重置访问标记（每次BFS前都需要重置）
        std::fill(visited, visited + n, false);

        // 使用BFS寻找增广路径
        if (!bfs_adj(s, t, n, parent_node, parent_edge, visited))
        {
            break; // 找不到增广路径，结束循环
        }

        // 1. 回溯路径，找到路径上的最小残留容量（瓶颈容量）
        int path_flow = INF; // 路径流量初始化为无穷大
        for (int v = t; v != s; v = parent_node[v])
        {
            int edge_idx = parent_edge[v]; // 获取到达v的边索引
            // 更新路径流量为当前最小容量
            path_flow = std::min(path_flow, edge_pool[edge_idx].capacity);
        }

        // 2. 再次回溯路径，更新残留图
        for (int v = t; v != s; v = parent_node[v])
        {
            int edge_idx = parent_edge[v]; // 获取到达v的边索引

            // 更新正向边容量（减少）
            edge_pool[edge_idx].capacity -= path_flow;

            // 更新反向边容量（增加）
            int rev_edge_idx = edge_pool[edge_idx].rev; // 获取反向边索引
            edge_pool[rev_edge_idx].capacity += path_flow;
        }

        // 3. 将本次找到的流量累加到总流量
        max_flow += path_flow;
    }

    // 释放辅助数组内存
    delete[] parent_node;
    delete[] parent_edge;
    delete[] visited;

    return max_flow; // 返回计算的最大流
}
