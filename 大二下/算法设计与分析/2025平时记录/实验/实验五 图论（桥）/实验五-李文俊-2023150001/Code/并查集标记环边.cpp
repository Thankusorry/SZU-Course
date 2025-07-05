// 该函数用于查找无向图中的桥
// 1. 首先通过深度优先搜索（DFS）构建一棵生成树
// 2. 然后在生成树上使用并查集算法查找桥
void findBridges()
{
    // 遍历所有节点，对于每个未访问的节点
    for (int i = 0; i < numNodes; i++)
    {
        if (!visited[i])
        {
            is_bridge[i] = false; // 初始化当前节点不是桥
            BuildTree(i);         // 通过DFS构建生成树，并标记访问过的节点
        }
    }

    // 初始化并查集，每个节点的父节点初始化为其自身
    UnionFind uf(numNodes);

    // 遍历所有反向边（即在生成树中的边）
    for (int i = 0; i < bcnt; i++)
    {
        int u = back_edges[i].u; // 反向边的第一个节点
        int v = back_edges[i].v; // 反向边的第二个节点

        // 查找两个节点所在集合的根节点
        int ru = uf.find(u);
        int rv = uf.find(v);

        // 当两个节点不在同一个集合中时，进行合并
        while (ru != rv)
        {
            // 处理最深节点，向上回溯
            if (depth[ru] < depth[rv])
            {
                int temp = ru;
                ru = rv;
                rv = temp;
            }
            is_bridge[ru] = false; // 将当前边标记为非桥
            int next = father[ru]; // 获取当前节点的父节点
            uf.unite(ru, next);    // 合并两个集合
            ru = uf.find(next);    // 继续向上回溯
        }
    }
}
