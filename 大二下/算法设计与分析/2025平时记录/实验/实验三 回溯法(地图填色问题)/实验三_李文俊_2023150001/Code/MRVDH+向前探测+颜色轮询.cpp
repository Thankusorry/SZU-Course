void mrv_dh_fc_lx(int current)
{
    if (cnt >= 400000)
        return;
    // if(cnt>= 10000000) return;
    if (current == -1)
    {
        cnt++; // 直接计数，无需乘阶乘
        return;
    }

    // 关键修改：对第一个节点限制颜色选择为1
    if (current == 96)
    {
        // 如果颜色1不可用，直接返回
        if (!domains[current].colors[1])
            return;

        // 强制第一个节点为颜色1
        color[current] = 1;
        RemovedColors rc;
        rc.nodes = new int[graph_size[current] * colornum];
        rc.colors = new int[graph_size[current] * colornum];
        rc.count = 0;

        bool fc_ok = forward_check(current, 1, &rc);
        if (fc_ok)
            mrv_dh_fc_lx(select_mrv_dh_node());

        // 恢复颜色域
        color[current] = 0;
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
        delete[] rc.nodes;
        delete[] rc.colors;
    }
    else
    {
        // 其他节点正常处理，但需限制颜色选择范围
        for (int c = 1; c <= colornum; ++c)
        {
            if (!domains[current].colors[c])
                continue;

            color[current] = c;
            RemovedColors rc;
            rc.nodes = new int[graph_size[current] * colornum];
            rc.colors = new int[graph_size[current] * colornum];
            rc.count = 0;

            bool fc_ok = forward_check(current, c, &rc);
            if (fc_ok)
                mrv_dh_fc_lx(select_mrv_dh_node());

            color[current] = 0;
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
            delete[] rc.nodes;
            delete[] rc.colors;
        }
    }
}