/*
 * 约束传播检查函数（CSP核心算法）
 * 参数：
 *   current - 当前处理的节点
 *   c - 当前尝试的颜色
 *   rc - 记录颜色删除和强制赋值的结构体
 * 返回值：
 *   true - 约束传播成功
 *   false - 遇到冲突需要回溯
 */
bool csp_check(int current, int c, RemovedColors *rc)
{
    // 使用固定大小数组模拟队列（比STL queue更高效）
    int *queue = new int[n + 1]; // 队列容量n+1防止溢出
    int front = 0, rear = 0;     // 队列头尾指针

    /* ===== 第一阶段：直接约束传播 ===== */
    for (int i = 0; i < graph_size[current]; ++i)
    {
        int neighbor = graph[current][i];
        // 只处理未着色且包含当前颜色的邻居
        if (color[neighbor] == 0 && domains[neighbor].colors[c])
        {
            // 记录颜色删除操作（用于回溯恢复）
            rc->nodes[rc->count] = neighbor;
            rc->colors[rc->count] = c;
            rc->count++;

            // 更新邻居颜色域
            domains[neighbor].colors[c] = false; // 移除冲突颜色
            domains[neighbor].size--;            // 可用颜色数减1

            // 剪枝条件：颜色域耗尽立即返回失败
            if (domains[neighbor].size == 0)
            {
                delete[] queue;
                return false;
            }

            // 检测单值节点（颜色域只剩1种选择）
            if (domains[neighbor].size == 1)
            {
                queue[rear++] = neighbor; // 入队等待后续处理
            }
        }
    }

    /* ===== 第二阶段：单值节点传播 ===== */
    while (front < rear)
    {
        int node = queue[front++]; // 取出队列头部节点

        // 寻找该节点的唯一可用颜色
        int single_color = 0;
        for (int col = 1; col <= colornum; ++col)
        {
            if (domains[node].colors[col])
            {
                single_color = col;
                break;
            }
        }

        // 记录强制赋值（用于回溯恢复）
        rc->forced_nodes[rc->forced_count] = node;
        rc->forced_colors[rc->forced_count] = single_color;
        rc->forced_count++;
        color[node] = single_color; // 自动填充颜色

        // 传播该节点的约束
        for (int i = 0; i < graph_size[node]; ++i)
        {
            int neighbor = graph[node][i];
            if (color[neighbor] == 0 && domains[neighbor].colors[single_color])
            {
                // 同上处理颜色删除和单值检测
                rc->nodes[rc->count] = neighbor;
                rc->colors[rc->count] = single_color;
                rc->count++;
                domains[neighbor].colors[single_color] = false;
                domains[neighbor].size--;
                if (domains[neighbor].size == 0)
                {
                    delete[] queue;
                    return false;
                }
                if (domains[neighbor].size == 1)
                {
                    queue[rear++] = neighbor;
                }
            }
        }
    }

    delete[] queue;
    return true;
}

/*
 * 约束传播回溯主函数
 * 参数：
 *   current - 当前处理的节点编号
 */
void csp(int current)
{
    // 终止条件：找到足够多的解
    if (cnt >= 100000)
        return;

    // 跳过已自动赋值的节点
    while (current <= n && color[current] != 0)
    {
        current++;
    }

    // 终止条件：所有节点完成着色
    if (current > n)
    {
        output(); // 输出当前解
        return;
    }

    // 遍历当前节点的可用颜色
    for (int c = 1; c <= colornum; ++c)
    {
        if (!domains[current].colors[c])
            continue;

        color[current] = c; // 临时着色

        // 初始化删除记录结构体
        RemovedColors rc;
        rc.nodes = new int[n * colornum]; // 预分配最大可能空间
        rc.colors = new int[n * colornum];
        rc.count = 0;
        rc.forced_nodes = new int[n]; // 强制赋值记录
        rc.forced_colors = new int[n];
        rc.forced_count = 0;

        // 执行约束传播检查
        bool ok = csp_check(current, c, &rc);

        if (ok)
        {
            // 递归处理下一个未赋值节点
            int next = current + 1;
            while (next <= n && color[next] != 0)
            {
                next++;
            }
            csp(next);
        }

        /* ===== 回溯恢复阶段 ===== */
        color[current] = 0; // 清除当前节点颜色

        // 恢复自动填充的节点
        for (int i = 0; i < rc.forced_count; ++i)
        {
            color[rc.forced_nodes[i]] = 0;
        }

        // 恢复颜色域
        for (int i = 0; i < rc.count; ++i)
        {
            int node = rc.nodes[i];
            int col = rc.colors[i];
            if (!domains[node].colors[col])
            {
                domains[node].colors[col] = true;
                domains[node].size++;
            }
        }

        // 释放内存
        delete[] rc.nodes;
        delete[] rc.colors;
        delete[] rc.forced_nodes;
        delete[] rc.forced_colors;
    }
}