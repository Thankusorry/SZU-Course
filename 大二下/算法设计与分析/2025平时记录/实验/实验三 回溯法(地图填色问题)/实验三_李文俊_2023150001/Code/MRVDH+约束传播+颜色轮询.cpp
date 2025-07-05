bool csp_check(int current, int c, RemovedColors* rc) {
    // 用数组模拟队列存储单值节点（假设最大队列长度为n）
    int* queue = new int[n + 1];
    int front = 0, rear = 0;

    // 第一步：处理当前节点的直接约束传播
    for (int i = 0; i < graph_size[current]; ++i) {
        int neighbor = graph[current][i];
        if (color[neighbor] == 0 && domains[neighbor].colors[c]) {
            // 记录删除操作
            rc->nodes[rc->count] = neighbor;
            rc->colors[rc->count] = c;
            rc->count++;
            domains[neighbor].colors[c] = false;
            domains[neighbor].size--;
            if (domains[neighbor].size == 0) {
                delete[] queue;
                return false;
            }
            // 检查是否变为单值节点
            if (domains[neighbor].size == 1) {
                queue[rear++] = neighbor;
            }
        }
    }

    // 第二步：处理所有单值节点队列
    while (front < rear) {
        int node = queue[front++];

        // 找到唯一颜色
        int single_color = 0;
        for (int col = 1; col <= colornum; ++col) {
            if (domains[node].colors[col]) {
                single_color = col;
                break;
            }
        }

        // 记录自动填充
        rc->forced_nodes[rc->forced_count] = node;
        rc->forced_colors[rc->forced_count] = single_color;
        rc->forced_count++;
        color[node] = single_color;

        // 传播该节点的约束
        for (int i = 0; i < graph_size[node]; ++i) {
            int neighbor = graph[node][i];
            if (color[neighbor] == 0 && domains[neighbor].colors[single_color]) {
                rc->nodes[rc->count] = neighbor;
                rc->colors[rc->count] = single_color;
                rc->count++;
                domains[neighbor].colors[single_color] = false;
                domains[neighbor].size--;
                if (domains[neighbor].size == 0) {
                    delete[] queue;
                    return false;
                }
                // 检查新的单值节点
                if (domains[neighbor].size == 1) {
                    queue[rear++] = neighbor;
                }
            }
        }
    }

    delete[] queue;
    return true;
}

void mrvdh_csp_lx(int current) {
    if(cnt>=400000) return;
    if (current  == -1 ) {
        // output();
        cnt++;
        return;
    }

    if(current == 220){

            color[current] = 1;
            RemovedColors rc;
            // 预分配足够空间（假设每个操作最多影响n个节点）
            rc.nodes = new int[n * colornum];
            rc.colors = new int[n * colornum];
            rc.count = 0;
            rc.forced_nodes = new int[n];
            rc.forced_colors = new int[n];
            rc.forced_count = 0;

            bool ok = csp_check(current, 1, &rc);

            if (ok) {
                // 递归处理下一个未赋值节点
                mrvdh_csp_lx(select_mrv_dh_node());
            }

            // 回溯恢复
            color[current] = 0;
            // 恢复自动填充的节点
            for (int i = 0; i < rc.forced_count; ++i) {
                color[rc.forced_nodes[i]] = 0;
            }
            // 恢复颜色域
            for (int i = 0; i < rc.count; ++i) {
                int node = rc.nodes[i];
                int col = rc.colors[i];
                if (!domains[node].colors[col]) {
                    domains[node].colors[col] = true;
                    domains[node].size++;
                }
            }
            delete[] rc.nodes;
            delete[] rc.colors;
            delete[] rc.forced_nodes;
            delete[] rc.forced_colors;
        }
    
    else{
        for (int c = 1; c <= colornum; ++c) {
            if (!domains[current].colors[c]) continue;
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

        if (ok) {
            // 递归处理下一个未赋值节点
            mrvdh_csp_lx(select_mrv_dh_node());
        }

        // 回溯恢复
        color[current] = 0;
        // 恢复自动填充的节点
        for (int i = 0; i < rc.forced_count; ++i) {
            color[rc.forced_nodes[i]] = 0;
        }
        // 恢复颜色域
        for (int i = 0; i < rc.count; ++i) {
            int node = rc.nodes[i];
            int col = rc.colors[i];
            if (!domains[node].colors[col]) {
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
