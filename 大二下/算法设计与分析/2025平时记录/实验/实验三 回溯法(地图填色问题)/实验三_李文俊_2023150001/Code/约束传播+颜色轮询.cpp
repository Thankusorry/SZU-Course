/*
 * 约束传播+LX优化回溯函数
 * 参数：
 *   current - 当前处理的节点编号
 * 功能：
 *   1. 对第一个节点采用固定颜色1的优化（LX优化）
 *   2. 对其他节点使用标准约束传播回溯
 *   3. 包含完整的前向检查和回溯恢复机制
 */
void csp_lx(int current)
{
    // 解数量达到阈值时提前终止（性能优化）
    if (cnt >= 6667)
        return;

    // 跳过已自动赋值的节点（约束传播可能自动填充某些节点）
    while (current <= n && color[current] != 0)
    {
        current++;
    }

    // 终止条件：所有节点完成着色
    if (current > n)
    {
        cnt++; // 解计数器递增
        return;
    }

    // ==== LX优化：固定第一个节点的颜色为1 ====
    if (current == 1)
    {
        color[current] = 1; // 固定着色1（利用对称性减少搜索空间）

        // 初始化删除记录结构体
        RemovedColors rc;
        rc.nodes = new int[n * colornum]; // 预分配最大可能空间
        rc.colors = new int[n * colornum];
        rc.count = 0;
        rc.forced_nodes = new int[n]; // 强制赋值记录
        rc.forced_colors = new int[n];
        rc.forced_count = 0;

        // 执行约束传播检查
        bool ok = csp_check(current, 1, &rc);

        if (ok)
        {
            // 递归处理下一个未赋值节点
            int next = current + 1;
            while (next <= n && color[next] != 0)
            {
                next++;
            }
            csp_lx(next);
        }

        /* ==== 回溯恢复阶段 ==== */
        color[current] = 0; // 清除当前节点颜色

        // 恢复自动填充的节点（约束传播强制赋值的节点）
        for (int i = 0; i < rc.forced_count; ++i)
        {
            color[rc.forced_nodes[i]] = 0;
        }

        // 恢复颜色域（撤销所有前向检查的删除操作）
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

    // ==== 标准约束传播处理其他节点 ====
    else
    {
        // 遍历当前节点的可用颜色
        for (int c = 1; c <= colornum; ++c)
        {
            if (!domains[current].colors[c])
                continue; // 跳过被排除的颜色

            color[current] = c; // 临时着色

            // 初始化删除记录（同上）
            RemovedColors rc;
            rc.nodes = new int[n * colornum];
            rc.colors = new int[n * colornum];
            rc.count = 0;
            rc.forced_nodes = new int[n];
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
                csp_lx(next);
            }

            /* ==== 回溯恢复阶段 ==== */
            color[current] = 0;
            for (int i = 0; i < rc.forced_count; ++i)
            {
                color[rc.forced_nodes[i]] = 0;
            }
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
            delete[] rc.nodes;
            delete[] rc.colors;
            delete[] rc.forced_nodes;
            delete[] rc.forced_colors;
        }
    }
}
