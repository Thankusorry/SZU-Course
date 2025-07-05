// ISAP算法 - 反向BFS初始化距离标号（邻接表版）
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
void isapInitDistance(int s, int t, int n)
{
    // 初始化距离数组为-1（表示未访问）
    std::fill(dist, dist + n, -1);
    // 初始化GAP数组为0
    std::fill(gap, gap + n + 1, 0);

    // 使用数组模拟队列（避免STL开销）
    int *queue = new int[n];
    int head1 = 0, tail = 0;

    // 从汇点开始BFS
    dist[t] = 0;       // 汇点距离设为0
    gap[0] = 1;        // 距离0的节点数设为1（汇点）
    queue[tail++] = t; // 汇点入队

    while (head1 < tail)
    {
        int u = queue[head1++]; // 出队一个节点

        // 遍历u的所有入边（通过其出边的反向边来模拟）
        for (int i = head[u]; i != -1; i = edge_pool[i].next)
        {
            Edge &e = edge_pool[i];
            int v = e.to; // 实际是u的入边起点

            // 如果v节点未被访问
            if (dist[v] == -1)
            {
                dist[v] = dist[u] + 1; // 设置v的距离（比u大1）
                gap[dist[v]]++;        // 更新该距离的节点计数
                queue[tail++] = v;     // v入队
            }
        }
    }
    delete[] queue; // 释放队列内存
}

// ISAP算法主函数（迭代式，邻接表版）
// 参数:
//   s: 源点
//   t: 汇点
//   n: 节点总数
// 返回值: 最大流值
int isap(int s, int t, int n)
{
    // 1. 初始化距离标号
    isapInitDistance(s, t, n);

    // 如果源点不可达（距离为-1），返回0流量
    if (dist[s] == -1)
        return 0;

    int max_flow = 0; // 最大流初始化为0
    int u = s;        // 当前节点设为源点

    // 分配路径记录数组（存储到达每个节点的边索引）
    int *parent_edge = new int[n];

    // 初始化当前弧优化数组（指向每个节点的邻接表头部）
    for (int i = 0; i < n; ++i)
        iter[i] = head[i];

    // 主循环：当源点距离小于n时继续（GAP优化可能提前结束）
    while (dist[s] < n)
    {
        // --- 情况1：到达汇点，找到增广路径 ---
        if (u == t)
        {
            int path_flow = INF; // 路径流量初始化为无穷大

            // 回溯计算瓶颈流量（从汇点回溯到源点）
            for (int curr = t; curr != s; curr = edge_pool[edge_pool[parent_edge[curr]].rev].to)
            {
                int edge_idx = parent_edge[curr]; // 获取到达curr的边索引
                path_flow = std::min(path_flow, edge_pool[edge_idx].capacity);
            }

            // 回溯更新残留网络（从汇点回溯到源点）
            for (int curr = t; curr != s; curr = edge_pool[edge_pool[parent_edge[curr]].rev].to)
            {
                int edge_idx = parent_edge[curr]; // 获取到达curr的边索引
                Edge &e = edge_pool[edge_idx];

                // 更新正向边容量（减少）
                e.capacity -= path_flow;
                // 更新反向边容量（增加）
                edge_pool[e.rev].capacity += path_flow;
            }

            max_flow += path_flow; // 累加路径流量
            u = s;                 // 重置当前节点为源点
        }

        // --- 情况2：寻找允许弧（满足距离标号差1的边） ---
        bool found_arc = false; // 是否找到允许弧

        // 当前弧优化：通过引用(&i)直接修改iter[u]的值
        for (int &i = iter[u]; i != -1; i = edge_pool[i].next)
        {
            Edge &e = edge_pool[i]; // 获取当前边的引用

            // 检查是否满足允许弧条件（有剩余容量且距离标号差1）
            if (e.capacity > 0 && dist[u] == dist[e.to] + 1)
            {
                parent_edge[e.to] = i; // 记录到达e.to的边索引
                u = e.to;              // 推进到下一节点
                found_arc = true;      // 标记找到允许弧
                break;                 // 跳出循环
            }
        }

        // --- 情况3：未找到允许弧（死胡同） ---
        if (!found_arc)
        {
            int old_dist = dist[u]; // 保存当前距离

            // a. 重标号前更新GAP数组
            gap[old_dist]--; // 当前距离的节点数减1

            // b. GAP优化：如果当前距离的节点数为0，出现断层
            if (gap[old_dist] == 0)
            {
                dist[s] = n; // 设置源点距离为n（结束算法）
            }

            // c. 重标号：寻找最小邻接距离+1
            dist[u] = n; // 初始化为最大值n
            for (int i = head[u]; i != -1; i = edge_pool[i].next)
            {
                Edge &e = edge_pool[i];
                // 只考虑有剩余容量的边
                if (e.capacity > 0)
                {
                    // 更新为最小邻接距离+1
                    dist[u] = std::min(dist[u], dist[e.to] + 1);
                }
            }

            // d. 更新GAP数组（如果新距离有效）
            if (dist[u] < n)
            {
                gap[dist[u]]++; // 新距离的节点数加1
            }

            // e. 重置当前弧（因为距离标号改变了）
            iter[u] = head[u];

            // f. 回溯：如果不是源点，回溯到父节点
            if (u != s)
            {
                // 通过反向边找到父节点
                u = edge_pool[edge_pool[parent_edge[u]].rev].to;
            }
        }
    }

    // 释放路径记录数组
    delete[] parent_edge;
    return max_flow; // 返回计算的最大流
}
