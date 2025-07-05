// 朴素回溯法实现地图填色
// 检查当前节点x的颜色是否与已着色邻居冲突
bool check(int x)
{
    // 遍历节点x的所有邻居（邻接表存储）
    for (int i = 0; i < graph_size[x]; i++)
    {
        int neighbor = graph[x][i];

        // 只检查已着色的邻居（编号小于x的节点）
        if (neighbor < x && color[x] == color[neighbor])
            return false; // 颜色冲突

        // 提前终止条件：遇到未处理的邻居（后续会处理）
        if (neighbor >= x)
            break;
    }
    return true; // 当前颜色选择有效
}

// 回溯法主函数
void normal_backtrack(int x)
{
    // 终止条件：所有n个节点完成着色
    if (x > n)
    {
        cnt++; // 解计数器+1
        return;
    }

    // 遍历所有可用颜色（1到colornum）
    for (int c = 1; c <= colornum; c++)
    {
        color[x] = c; // 尝试为当前节点选择颜色c

        // 如果当前颜色选择有效
        if (check(x))
        {
            normal_backtrack(x + 1); // 递归处理下一个节点
        }

        color[x] = 0; // 回溯：撤销当前节点的颜色选择
    }
}
