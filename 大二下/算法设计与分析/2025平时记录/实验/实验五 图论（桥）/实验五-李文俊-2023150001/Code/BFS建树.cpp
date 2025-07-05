// 该函数用于构建图树的算法，采用深度优先搜索（DFS）
// 参数 root 是树的根节点
void BuildTree(int root)
{
    // 标记当前节点为已访问
    visited[root] = true;

    // 遍历当前节点的所有邻居节点
    for (int i = 0; i < graphSizes[root]; i++)
    {
        int neighbor = graph[root][i];

        // 如果邻居节点已经是当前节点的父节点，则跳过（避免回边）
        if (neighbor == father[root])
            continue;
        // 如果邻居节点未被访问
        if (!visited[neighbor])
        {
            // 调试输出：记录邻居节点的父节点为当前节点
            father[neighbor] = root;
            // 计算邻居节点的深度
            depth[neighbor] = depth[root] + 1;
            // 标记邻居节点为桥接节点
            is_bridge[neighbor] = true;
            // 递归调用 BuildTree 函数，继续构建子树
            BuildTree(neighbor);
        }
        else
        { // 如果邻居节点已经在树中且不是直接前驱
            // 如果当前节点在字典序上小于邻居节点，记录为反向边
            if (root < neighbor)
            {
                back_edges[bcnt++] = {root, neighbor};
                // 调试输出：记录反向边
                // cout<<"root:"<<root<<" neighbor:"<<neighbor<<endl;
            }
        }
    }
}
