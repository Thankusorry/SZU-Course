// 定义函数isBridge，用于判断图中的边(u, v)是否为桥
bool isBridge(int u, int v)
{
    // 动态分配一个布尔数组，用于记录节点是否被访问过
    bool *visited = new bool[numNodes];
    // 将所有节点初始化为未访问状态
    memset(visited, false, sizeof(bool) * numNodes);
    // 初始化队列，用于广度优先搜索
    queue<int> q;
    // 将起始节点u加入队列，并将其标记为已访问
    q.push(u);
    visited[u] = true;

    // 当队列不为空时，继续搜索
    while (!q.empty())
    {
        // 取出队列的第一个元素
        int current = q.front();
        q.pop();
        // 如果当前节点是目标节点v，说明边(u, v)不是桥，返回false
        if (current == v)
        {
            delete[] visited;
            return false;
        }
        // 遍历当前节点的所有邻居节点
        for (int i = 0; i < graphSizes[current]; i++)
        {
            int neighbor = graph[current][i];
            // 跳过边u-v和v-u，因为这两条边在搜索过程中会被自然覆盖
            if ((current == u && neighbor == v) || (current == v && neighbor == u))
            {
                continue;
            }
            // 如果邻居节点未被访问过
            if (!visited[neighbor])
            {
                // 标记邻居节点为已访问
                visited[neighbor] = true;
                // 将邻居节点加入队列
                q.push(neighbor);
                // 输出调试信息，表示将邻居节点加入队列
                // cout<<"push neighbor="<<neighbor<<endl;
            }
        }
    }
    // 如果遍历完所有可达节点仍未找到节点v，说明边(u, v)是桥，返回true
    // 但在这个实现中，这种情况不会发生，因为总会有一个路径到达v
    delete[] visited;
    return true;
}
