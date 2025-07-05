/*
 * 二次前向检查函数（双重约束传播）
 * 参数：
 *   current - 当前处理的节点
 *   c - 尝试的颜色值
 *   rc - 记录颜色删除操作的结构体
 * 返回值：
 *   true - 检查通过，可以继续递归
 *   false - 检测到冲突，需要回溯
 */
bool forward_check2(int current, int c, RemovedColors *rc)
{
    // 遍历当前节点的所有邻接节点
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

            // 剪枝条件1：邻居颜色域耗尽
            if (domains[neighbor].size == 0)
                return false;

            // 检测单值节点（颜色域只剩1种选择）
            if (domains[neighbor].size == 1)
            {
                // 找出该邻居的唯一可用颜色
                int single_color = 0;
                for (int col = 1; col <= colornum; ++col)
                {
                    if (domains[neighbor].colors[col])
                    {
                        single_color = col;
                        break;
                    }
                }

                /* 二次检查：验证该颜色在二级邻居中的合法性 */
                for (int j = 0; j < graph_size[neighbor]; ++j)
                {
                    int neighbor2 = graph[neighbor][j]; // 获取二级邻居
                    // 如果二级邻居也是单值节点且颜色冲突
                    if (color[neighbor2] == 0 &&
                        domains[neighbor2].size == 1 &&
                        domains[neighbor2].colors[single_color])
                    {
                        return false; // 剪枝条件2：二级冲突
                    }
                }
            }
        }
    }
    return true; // 所有检查通过
}

/*
 * 二次前向检查的回溯主函数
 * 参数：
 *   current - 当前处理的节点编号
 */
void backtrack_forward2(int current)
{
    // 终止条件：完成所有节点着色
    if (current > n)
    {
        cnt++; // 解计数器递增
        return;
    }

    // 遍历当前节点的可用颜色
    for (int c = 1; c <= colornum; ++c)
    {
        // 跳过被排除的颜色
        if (!domains[current].colors[c])
            continue;

        color[current] = c; // 临时着色

        // 初始化删除记录结构体
        RemovedColors rc;
        rc.nodes = new int[graph_size[current] * colornum]; // 预分配最大可能空间
        rc.colors = new int[graph_size[current] * colornum];
        rc.count = 0; // 初始化计数器

        // 执行二次前向检查
        bool fc_ok = forward_check2(current, c, &rc);

        // 检查通过则递归处理下一个节点
        if (fc_ok)
        {
            backtrack_forward2(current + 1);
        }

        /* ==== 回溯恢复阶段 ==== */
        color[current] = 0; // 清除当前节点颜色

        // 恢复所有被删除的颜色选项
        for (int i = 0; i < rc.count; ++i)
        {
            int node = rc.nodes[i];
            int color_val = rc.colors[i];
            // 避免重复恢复（幂等操作）
            if (!domains[node].colors[color_val])
            {
                domains[node].colors[color_val] = true;
                domains[node].size++; // 颜色域大小恢复
            }
        }

        // 释放临时内存
        delete[] rc.nodes;
        delete[] rc.colors;
    }
}
