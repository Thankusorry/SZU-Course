struct UnionFind
{

    int *parent;
    UnionFind(int size)
    {
        parent = new int[size];
        rank = new int[size];
        for (int i = 0; i < size; ++i)
        {
            parent[i] = i;
        }
    }

    // 找祖先
    inline int find(int x)
    {
        int root = x;
        // 递推求根节点
        while (parent[root] != root)
        {
            root = parent[root];
        }
        // 将所有子节点都接到根节点
        int curr = x;
        while (parent[curr] != root)
        {
            int next = parent[curr];
            parent[curr] = root;
            curr = next;
        }
        return root;
    }

    inline bool unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY)
        {
            parent[rootX] = rootY;
            return true;
        }
        return false;
    }

    bool connected(int x, int y)
    {
        return find(x) == find(y);
    }
};