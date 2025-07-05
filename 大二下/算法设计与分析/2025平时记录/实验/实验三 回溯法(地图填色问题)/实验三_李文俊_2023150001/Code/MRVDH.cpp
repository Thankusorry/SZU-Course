/* MRV+DH启发式节点选择算法
 * 返回值：下一个要处理的节点编号
 * 策略：优先选择颜色域最小的节点(MRV)，当多个节点域大小相同时选择度数最大的(DH)
 */
int select_mrv_dh_node()
{
    int mrv_node = -1;
    int min_size = colornum + 1; // 初始化为超过最大可能值
    int maxdrgee = -1;

    // 第一阶段：寻找最小颜色域
    for (int i = 1; i <= n; ++i)
    {
        // 只处理未着色节点
        if (color[i] == 0 && domains[i].size < min_size)
        {
            min_size = domains[i].size;
            mrv_node = i;
        }
    }

    // 第二阶段：在MRV候选集中选择最大度节点
    for (int i = 1; i <= n; ++i)
    {
        if (color[i] == 0 && domains[i].size == min_size)
        {
            // 比较节点的度数（即邻接节点数量）
            if (graph_size[i] > maxdrgee)
            {
                maxdrgee = graph_size[i];
                mrv_node = i;
            }
        }
    }
    return mrv_node;
}

/* MRV+DH启发式回溯算法
 * 参数：current - 当前处理的节点编号
 * 功能：递归尝试所有可能颜色，执行前向检查并维护颜色域
 */
void backtrack_mrv_dh(int current)
{
    if (cnt >= 10000000)
        return; // 解数量安全阀

    // 终止条件：所有节点已处理
    if (current == -1)
    {
        cnt++; // 合法解计数器递增
        return;
    }

    // 遍历当前节点的所有可能颜色
    for (int c = 1; c <= colornum; ++c)
    {
        // 跳过不可用颜色
        if (!domains[current].colors[c])
            continue;

        // 尝试当前颜色
        color[current] = c;

        // 前向检查记录容器（预分配最大可能空间）
        RemovedColors rc;
        rc.nodes = new int[graph_size[current] * colornum];
        rc.colors = new int[graph_size[current] * colornum];
        rc.count = 0;

        // 前向检查：删除邻居节点的当前颜色
        for (int i = 0; i < graph_size[current]; ++i)
        {
            int neighbor = graph[current][i];
            // 仅处理未着色且包含当前颜色的邻居
            if (color[neighbor] == 0 && domains[neighbor].colors[c])
            {
                // 记录删除操作（用于回溯恢复）
                rc.nodes[rc.count] = neighbor;
                rc.colors[rc.count] = c;
                rc.count++;
                // 更新邻居颜色域
                domains[neighbor].colors[c] = false;
                domains[neighbor].size--;
            }
        }

        // 递归处理下一个节点（通过启发式选择）
        backtrack_mrv_dh(select_mrv_dh_node());

        // 回溯恢复阶段
        color[current] = 0; // 清除当前节点颜色
        // 还原邻居节点的颜色域
        for (int i = 0; i < rc.count; ++i)
        {
            int node = rc.nodes[i];
            int color_val = rc.colors[i];
            if (!domains[node].colors[color_val])
            {
                domains[node].colors[color_val] = true;
                domains[node].size++;
            }
        }
        // 释放临时内存
        delete[] rc.nodes;
        delete[] rc.colors;
    }
}
