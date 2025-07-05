// 前向检查函数：检查当前节点着色对后续节点的影响
// current - 当前处理节点，c - 尝试的颜色，rc - 记录颜色删除操作的结构体
bool forward_check(int current, int c, RemovedColors *rc)
{
    // 遍历当前节点的所有邻居节点
    for (int i = 0; i < graph_size[current]; ++i)
    {
        int neighbor = graph[current][i];
        // 仅处理未着色邻居（邻接表存储）
        if (color[neighbor] == 0 && domains[neighbor].colors[c])
        {
            // 记录被删除颜色的节点和颜色值（用于回溯恢复）
            rc->nodes[rc->count] = neighbor;
            rc->colors[rc->count] = c;
            rc->count++;

            // 更新邻居节点的颜色域
            domains[neighbor].colors[c] = false; // 删除冲突颜色
            domains[neighbor].size--;            // 颜色域大小减1

            // 剪枝条件：如果邻居颜色域耗尽则立即返回失败
            if (domains[neighbor].size == 0)
                return false;
        }
    }
    return true; // 前向检查通过
}

// 带前向探查的回溯主函数
void backtrack_forward(int current)
{
    // 终止条件：所有节点完成着色
    if (current > n)
    {
        cnt++; // 解计数器递增
        return;
    }

    // 遍历所有可用颜色（通过颜色域过滤）
    for (int c = 1; c <= colornum; ++c)
    {
        if (!domains[current].colors[c]) // 跳过已排除的颜色
            continue;

        color[current] = c; // 临时着色

        // 创建删除记录结构体（动态分配保证递归安全）
        RemovedColors rc;
        rc.nodes = new int[graph_size[current] * colornum]; // 预分配最大可能空间
        rc.colors = new int[graph_size[current] * colornum];
        rc.count = 0; // 初始化删除计数器

        // 执行前向检查并获取结果
        bool fc_ok = forward_check(current, c, &rc);

        if (fc_ok) // 前向检查通过才递归
        {
            backtrack_forward(current + 1); // 处理下一个节点
        }

        // 回溯恢复阶段
        color[current] = 0; // 清除当前节点颜色
        // 逆向恢复所有被删除的颜色选项
        for (int i = 0; i < rc.count; ++i)
        {
            int node = rc.nodes[i];
            int color_val = rc.colors[i];
            if (!domains[node].colors[color_val]) // 避免重复恢复
            {
                domains[node].colors[color_val] = true;
                domains[node].size++; // 颜色域大小复原
            }
        }

        // 释放临时内存
        delete[] rc.nodes;
        delete[] rc.colors;
    }
}
