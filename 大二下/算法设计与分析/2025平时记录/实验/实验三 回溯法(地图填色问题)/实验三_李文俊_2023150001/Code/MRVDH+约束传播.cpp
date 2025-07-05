void mrvdh_csp(int current)
{
    if (cnt >= 10000000)
        return;
    if (current == -1)
    {
        // output();
        cnt++;
        return;
    }

    for (int c = 1; c <= colornum; ++c)
    {
        if (!domains[current].colors[c])
            continue;
        color[current] = c;
        RemovedColors rc;
        // 预分配足够空间（假设每个操作最多影响n个节点）
        rc.nodes = new int[n * colornum];
        rc.colors = new int[n * colornum];
        rc.count = 0;
        rc.forced_nodes = new int[n];
        rc.forced_colors = new int[n];
        rc.forced_count = 0;

        bool ok = csp_check(current, c, &rc);

        if (ok)
        {
            // 递归处理下一个未赋值节点
            mrvdh_csp(select_mrv_dh_node());
        }

        // 回溯恢复
        color[current] = 0;
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
        delete[] rc.nodes;
        delete[] rc.colors;
        delete[] rc.forced_nodes;
        delete[] rc.forced_colors;
    }
}