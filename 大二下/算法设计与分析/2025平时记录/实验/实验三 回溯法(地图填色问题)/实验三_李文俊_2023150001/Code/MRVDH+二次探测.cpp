// 前向检查并记录删除的颜色
bool forward_check2(int current, int c, RemovedColors *rc)
{
    for (int i = 0; i < graph_size[current]; ++i)
    {
        int neighbor = graph[current][i];
        if (color[neighbor] == 0 && domains[neighbor].colors[c])
        {
            // 记录删除操作
            rc->nodes[rc->count] = neighbor;
            rc->colors[rc->count] = c;
            rc->count++;
            // 更新颜色域
            domains[neighbor].colors[c] = false;
            domains[neighbor].size--;
            if (domains[neighbor].size == 0)
                return false;
            if (domains[neighbor].size == 1)
            {
                int single_color = 0;
                for (int col = 1; col <= colornum; ++col)
                {
                    if (domains[neighbor].colors[col])
                    {
                        single_color = col;
                        break;
                    }
                }

                // 检查neighbor的邻接点是否也只剩single_color
                for (int j = 0; j < graph_size[neighbor]; ++j)
                {
                    int neighbor2 = graph[neighbor][j];
                    if (color[neighbor2] == 0 && domains[neighbor2].size == 1 &&
                        domains[neighbor2].colors[single_color])
                    {
                        return false; // 直接返回失败
                    }
                }
            }
        }
    }
    return true;
}
// 二次探测
void backtrack_forward2_mrvdh(int current)
{
    // if(cnt>=100000) return;
    if (cnt >= 10000000)
        return;
    if (current == -1)
    {
        // output();
        cnt++;
        return;
    }
    // 尝试所有可用颜色
    for (int c = 1; c <= colornum; ++c)
    {
        if (!domains[current].colors[c])
            continue;

        color[current] = c;
        // 准备回溯记录
        RemovedColors rc;
        rc.nodes = new int[graph_size[current] * colornum]; // 最大可能空间
        rc.colors = new int[graph_size[current] * colornum];
        rc.count = 0;

        bool fc_ok = forward_check2(current, c, &rc);

        if (fc_ok)
        {
            backtrack_forward2_mrvdh(select_mrv_dh_node());
        }

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
}