#include <bits/stdc++.h>
#include <chrono>
using namespace std;

struct edge
{
    int u;
    int v;
};

int numNodes, numEdges;
int **graph;
int *graphSizes;
edge *E;
int *depth;
bool *visited;
int *father;
bool *is_bridge;
edge *back_edges; //=new edge[133];
int bcnt = 0;     // 记录反向边数量
int tcnt = 0;     // Tree size
int cnt = 0;      // Bridge size

double time1 = 0, time2 = 0, time3 = 0, time4 = 0;
// 自定义边哈希函数
struct UnionFind
{

    int *parent;
    UnionFind(int size)
    {
        parent = new int[size];
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

        // 将所有父节点都接到根节点
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
void BuildTree(int root)
{
    static int stack[300000];
    int top = 0;
    stack[top++] = root;
    visited[root] = true;

    while (top > 0)
    {
        int u = stack[--top]; // 查看栈顶不弹出

        // 遍历邻居（从上次位置继续）
        for (int i = 0; i < graphSizes[u]; i++)
        {
            int v = graph[u][i];
            if (v == father[u])
                continue;

            if (!visited[v])
            {
                visited[v] = true;
                father[v] = u;
                depth[v] = depth[u] + 1;
                stack[top++] = v; // 压栈
            }
            else if (u < v)
            { // 后向边
                back_edges[bcnt++] = {u, v};
            }
        }
    }
}

void findBridges()
{
    auto start2 = chrono::high_resolution_clock::now();
    for (int i = 0; i < numNodes; i++)
    {
        if (!visited[i])
        {
            is_bridge[i] = false;
            BuildTree(i);
        }
    }
    auto end2 = chrono::high_resolution_clock::now();
    time1 = chrono::duration_cast<chrono::microseconds>(end2 - start2).count();
    auto start3 = chrono::high_resolution_clock::now();
    UnionFind uf(numNodes);
    for (int i = 0; i < bcnt; i++)
    {
        int u = back_edges[i].u;
        int v = back_edges[i].v;
        int ru = uf.find(u);
        int rv = uf.find(v);

        while (ru != rv)
        {
            // 处理最深节点 向上回溯
            if (depth[ru] < depth[rv])
            {
                int temp = ru;
                ru = rv;
                rv = temp;
            }
            is_bridge[ru] = false;
            int next = father[ru];
            uf.unite(ru, next);
            ru = uf.find(next);
        }
    }
    auto end3 = chrono::high_resolution_clock::now();
    time2 = chrono::duration_cast<chrono::microseconds>(end3 - start3).count();
}
vector<pair<int, int>> generate_undirected_graph(int n, long long m)
{
    long long max_edges = (long long)n * (n - 1) / 2;
    if (m > max_edges)
    {
        throw runtime_error("边数超过了最大可能值: " + to_string(max_edges));
    }

    vector<pair<int, int>> edges;

    if (m * 2 > max_edges)
    {
        // 如果边数较多，生成所有可能边再采样
        vector<pair<int, int>> all_edges;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                all_edges.emplace_back(i, j);
            }
        }

        // 随机打乱所有边
        shuffle(all_edges.begin(), all_edges.end(), mt19937(random_device{}()));

        // 取前m条边
        edges.assign(all_edges.begin(), all_edges.begin() + m);
    }
    else
    {
        // 边数较少时直接随机生成
        set<pair<int, int>> edge_set;
        uniform_int_distribution<int> dist(0, n - 1);
        mt19937 rng(random_device{}());

        while (edge_set.size() < (size_t)m)
        {
            int u = dist(rng);
            int v = dist(rng);
            if (u == v)
                continue;

            // 确保小的节点在前
            if (u > v)
                swap(u, v);
            edge_set.insert(make_pair(u, v));
        }

        edges.assign(edge_set.begin(), edge_set.end());
    }

    return edges;
}

void write_graph_to_file(int n, long long m, const vector<pair<int, int>> &edges)
{
    string filename = to_string(n) + "_" + to_string(m) + ".txt";
    ofstream outfile(filename);
    if (!outfile.is_open())
    {
        throw runtime_error("无法创建文件: " + filename);
    }

    // 写入节点数和边数
    outfile << n << " " << m << "\n";

    // 写入所有边
    for (const auto &edge : edges)
    {
        outfile << edge.first << " " << edge.second << "\n";
    }

    outfile.close();
}

int main()
{
    // // 生成随机图
    int n; // 点
    int m; // 边

    n = 600;
    // m = n;
    // m=n*n/4;
    // auto Edges = generate_undirected_graph(n, m);
    // write_graph_to_file(n, m, Edges);
    // cout << "文件已生成: " << n << "_" << m << ".txt\n";

    // // 读取文件
    // ifstream file(to_string(n) + "_" + to_string(m) + ".txt");

    // std::ifstream file("small.txt");
    // std::ifstream file("medium.txt");
    std::ifstream file("large.txt");
    if (!file.is_open())
    {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }
    // 并查集

    file >> numNodes; // 读取节点数
    file >> numEdges; // 读取边数

    vector<vector<int>> edges(numNodes + 1);
    E = new edge[numEdges + 1];
    back_edges = new edge[numEdges + 1];
    depth = new int[numNodes + 1];
    visited = new bool[numNodes + 1];
    father = new int[numNodes + 1];
    is_bridge = new bool[numEdges + 1];
    for (int i = 0; i < numNodes; i++)
    {
        visited[i] = false;
        is_bridge[i] = true;
        depth[i] = 0;
    }
    int u, v, num = 0;

    for (int i = 0; i < numEdges; ++i)
    {
        file >> u >> v; // 读取每条边的两个顶点
        if (u == v)
            continue;
        if (!count(edges[u].begin(), edges[u].end(), v))
        {
            edges[u].push_back(v);
            edges[v].push_back(u);
            E[num++] = {u, v};
        }
    }

    for (int i = 0; i < numNodes; i++)
    {
        sort(edges[i].begin(), edges[i].end());
    }
    numEdges = num;
    cout << "numNodes=" << numNodes << endl;
    cout << "numEdges=" << numEdges << endl;

    file.close();
    graph = new int *[numNodes + 1]; // 邻接表
    graphSizes = new int[numNodes + 1];
    for (int i = 0; i < numNodes; i++)
    {
        int num = edges[i].size();
        graphSizes[i] = num;
        graph[i] = new int[num];
        for (int j = 0; j < num; j++)
        {
            graph[i][j] = edges[i][j];
        }
    }

    /////// 生成树////////////////////

    auto start1 = chrono::high_resolution_clock::now();
    findBridges();
    auto end1 = chrono::high_resolution_clock::now();

    // 输出结果

    for (int v = 0; v < numNodes; ++v)
    {
        if (is_bridge[v])
        {
            cout << father[v] << "-" << v << endl;
            cnt++;
        }
    }
    cout << "Bridge:" << cnt << endl;
    cout << "Tree + 并查集: "
         << chrono::duration_cast<chrono::microseconds>(end1 - start1).count()
         << "μs" << endl;
    cout << "BuildTree time:" << time1 << endl;
    cout << "UnionFind time:" << time2 << endl;

    // 释放内存
    for (int i = 0; i < numNodes; i++)
    {
        delete[] graph[i];
    }
    delete[] depth;
    delete[] visited;
    delete[] back_edges;
    delete[] is_bridge;
    delete[] graph;
    delete[] graphSizes;
    delete[] E;
    delete[] father;
    return 0;
}