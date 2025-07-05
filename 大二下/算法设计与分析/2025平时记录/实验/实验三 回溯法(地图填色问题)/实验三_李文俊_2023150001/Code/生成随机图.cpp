#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class GraphGenerator
{
private:
    int n;                        // 节点数
    int target_edges;             // 目标边数
    vector<pair<int, int>> edges; // 边集合
    set<pair<int, int>> edge_set; // 快速查找边

    // 并查集用于确保连通性
    struct UnionFind
    {
        vector<int> parent;
        UnionFind(int size) : parent(size)
        {
            for (int i = 0; i < size; ++i)
                parent[i] = i;
        }
        int find(int x)
        {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        }
        bool unite(int x, int y)
        {
            int fx = find(x), fy = find(y);
            if (fx == fy)
                return false;
            parent[fy] = fx;
            return true;
        }
    };

public:
    GraphGenerator(int nodes, double avg_degree) : n(nodes)
    {
        target_edges = (avg_degree * nodes) / 2;
    }

    bool generate()
    {
        // 验证参数有效性
        if (target_edges < n - 1 || target_edges > 3 * n - 6)
        {
            cerr << "无效参数: 边数需在 [" << n - 1 << ", " << 3 * n - 6 << "] 之间\n";
            return false;
        }

        // 初始化随机种子
        srand(time(0));

        // 创建生成树保证连通性
        UnionFind uf(n);
        edges.clear();
        edge_set.clear();

        // 生成生成树边
        for (int i = 1; i < n; ++i)
        {
            int parent = rand() % i;
            add_edge(parent, i);
            uf.unite(parent, i);
        }

        // 添加随机边直到达到目标数
        while (edges.size() < target_edges)
        {
            int u = rand() % n;
            int v = rand() % n;
            if (u != v && !edge_exists(u, v))
            {
                add_edge(u, v);
            }
        }

        return true;
    }

    void save_to_file(const string &filename)
    {
        ofstream fout(filename);
        fout << n << " " << edges.size() << "\n";
        for (auto &e : edges)
        {
            fout << "e " << e.first + 1 << " " << e.second + 1 << "\n";
        }
        fout.close();
    }

private:
    void add_edge(int u, int v)
    {
        if (u > v)
            swap(u, v);
        edges.emplace_back(u, v);
        edge_set.emplace(u, v);
    }

    bool edge_exists(int u, int v)
    {
        if (u > v)
            swap(u, v);
        return edge_set.count({u, v});
    }
};

int main()
{
    int nodes;
    double avg_degree;

    nodes = 2000;
    avg_degree = 5;

    GraphGenerator generator(nodes, avg_degree);
    if (generator.generate())
    {
        generator.save_to_file("map1000_5.1.txt");
        cout << "图形已生成并保存到 graph.txt\n";
    }
    else
    {
        cout << "图形生成失败\n";
    }

    return 0;
}