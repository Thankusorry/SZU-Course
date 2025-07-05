// 这段代码用于计算图中非桥边的数量

// 外层循环遍历每一条边
for (int i = 0; i < numEdges; ++i)
{
    int u = E[i].u; // 获取当前边的两个顶点
    int v = E[i].v;
    // 创建一个新的并查集实例，初始时每个节点自成一个集合
    UnionFind uf(numNodes);
    // 内层循环遍历所有边，尝试将当前边之外的边进行合并
    for (int j = 0; j < numEdges; ++j)
    {
        if (j == i)
            continue;   // 排除当前边，避免重复合并
        int a = E[j].u; // 获取另一条边的两个顶点
        int b = E[j].v;
        uf.unite(a, b); // 合并两个节点所在的集合
    }
    // 如果当前边(u, v)不在同一个集合中，说明它不是桥
    if (!uf.connected(u, v))
    {
        // 输出当前边的信息（可根据需要取消注释）
        // cout<< u << " "<< v << endl;
        cnt++; // 非桥边的数量加一
    }
}
