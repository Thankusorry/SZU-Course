/*
 * 地图着色问题主程序
 * 功能：
 *   1. 支持从.col文件或生成随机图读取数据
 *   2. 实现多种回溯算法框架
 *   3. 包含性能测试和结果验证功能
 */

#include <bits/stdc++.h>
using namespace std;

/* ===== 核心数据结构 ===== */
struct Domain {
    bool *colors; // 颜色可用性数组（colors[c]表示颜色c是否可用）
    int size;     // 当前可用颜色数量
};

struct RemovedColors {
    int *nodes;  // 被删除颜色的节点数组
    int *colors; // 被删除的颜色值数组
    int count;   // 删除操作计数
    
    int* forced_nodes;  // 自动填充的节点（约束传播产生）
    int* forced_colors; // 自动填充的颜色
    int forced_count;   // 自动填充计数
};

/* ===== 全局变量 ===== */
int n;           // 节点总数
int **graph;     // 邻接表（graph[i]指向节点i的邻接数组）
int *graph_size; // 每个节点的邻居数
Domain *domains; // 每个节点的颜色域
int *color;      // 节点当前颜色（0表示未着色）
int cnt = 0;     // 合法方案计数器
int colornum;    // 颜色总数

/* ===== 图数据初始化 ===== */
void init() {
    // 文件选择示例（根据测试需求取消注释）
    // ifstream fin("le450_5a.col"); colornum = 5;
    ifstream fin("map500_5.1.txt"); colornum=12;
    
    if (!fin) {
        cerr << "文件打开失败" << endl;
        exit(1);
    }

    /* 文件解析流程 */
    string line;
    int edges = 0;
    // 解析图规模（p edge 节点数 边数）
    while (getline(fin, line)) {
        if (line[0] == 'p') {
            istringstream iss(line);
            string tmp;
            iss >> tmp >> tmp >> n >> edges;
            // 动态分配内存
            graph = new int*[n + 1];
            graph_size = new int[n + 1]{0};
            color = new int[n + 1]{0};
            break;
        }
    }

    /* 邻接表构建 */
    vector<vector<int>> temp_graph(n + 1);
    while (getline(fin, line)) {
        if (line[0] == 'e') {  // 边数据行
            int u, v;
            sscanf(line.c_str(), "e %d %d", &u, &v);
            temp_graph[u].push_back(v);
            temp_graph[v].push_back(u);
        }
    }

    // 转换为排序后的邻接数组
    for (int i = 1; i <= n; ++i) {
        sort(temp_graph[i].begin(), temp_graph[i].end());
        graph_size[i] = temp_graph[i].size();
        graph[i] = new int[graph_size[i]];
        copy(temp_graph[i].begin(), temp_graph[i].end(), graph[i]);
    }

    /* 初始化颜色域 */
    domains = new Domain[n + 1];
    for (int i = 1; i <= n; ++i) {
        domains[i].colors = new bool[colornum + 1]; // 颜色编号1~colornum
        domains[i].size = colornum;
        fill(domains[i].colors + 1, domains[i].colors + colornum + 1, true);
    }
    fin.close();
}

/* ===== 测试用图初始化 ===== */
void init1() {
    n = 9;  // 9节点测试图
    colornum = 4;
    /* 硬编码邻接关系 */
    int edges[][2] = {{1,2},{1,3},{1,4},{2,3},{2,4},{2,5},
                     {3,4},{4,5},{4,6},{4,9},{5,6},{5,7},
                     {6,7},{6,8},{6,9},{7,8},{8,9}};
    // ...（其余初始化代码与init()类似）
}

/* ===== 结果输出 ===== */
void output() {
    cout << "方案#" << ++cnt << ":\n";
    for (int i = 1; i <= n; i++)
        cout << color[i] << " ";
    cout << endl;
}

/* ===== 全局解验证 ===== */
bool whole_check() {
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < graph_size[i]; j++) {
            if (color[i] == color[graph[i][j]])
                return false;  // 邻接节点颜色冲突
        }
    }
    return true;
}

/* ===== 随机图生成器 ===== */
class GraphGenerator {
private:
    int n;                // 节点数
    int target_edges;     // 目标边数
    vector<pair<int,int>> edges;  // 边集合
    set<pair<int,int>> edge_set;  // 快速查询
    
    // 并查集（确保图连通性）
    struct UnionFind {
        vector<int> parent;
        UnionFind(int size) : parent(size) {
            iota(parent.begin(), parent.end(), 0);
        }
        int find(int x) { 
            return parent[x] == x ? x : (parent[x] = find(parent[x])); 
        }
        bool unite(int x, int y) {
            int fx = find(x), fy = find(y);
            if (fx == fy) return false;
            parent[fy] = fx;
            return true;
        }
    };

public:
    GraphGenerator(int nodes, double avg_degree) 
        : n(nodes), target_edges(avg_degree * nodes / 2) {}
    
    bool generate() {
        srand(time(0));
        UnionFind uf(n);
        
        // 生成生成树保证连通
        for (int i = 1; i < n; ++i) {
            int parent = rand() % i;
            add_edge(parent, i);
            uf.unite(parent, i);
        }
        
        // 添加随机边
        while (edges.size() < target_edges) {
            int u = rand() % n, v = rand() % n;
            if (u != v && !edge_exists(u, v)) 
                add_edge(u, v);
        }
        return true;
    }
    
    void save_to_file(const string& filename) {
        ofstream fout(filename);
        fout << "p edge " << n << " " << edges.size() << "\n";
        for (auto& e : edges) 
            fout << "e " << e.first+1 << " " << e.second+1 << "\n";
        fout.close();
    }
    
private:
    void add_edge(int u, int v) {
        if (u > v) swap(u, v);
        edges.emplace_back(u, v);
        edge_set.emplace(u, v);
    }
    
    bool edge_exists(int u, int v) {
        if (u > v) swap(u, v);
        return edge_set.count({u, v});
    }
};

/* ===== 主测试框架 ===== */
int main() {
    // 生成测试图（500节点，平均度5）
    GraphGenerator generator(500, 5);
    generator.generate();
    generator.save_to_file("map500_5.1.txt");
    
    // 性能测试循环（运行10次取平均）
    double sum_time = 0;
    for(int t = 1; t <= 10; t++) {
        init();  // 初始化图数据
        
        auto start = chrono::high_resolution_clock::now();
        // 选择算法执行（示例用MRV+DH启发式）
        int current = select_mrv_dh_node(); 
        backtrack_mrv_dh(current);
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        double elapsed = duration.count() / 1000.0;
        sum_time += elapsed;
        
        // 输出单次测试结果
        cout << "颜色数:" << colornum 
             << " | 用时:" << elapsed << "ms"
             << " | 方案数:" << cnt << endl;
             
        // 释放内存
        for (int i = 1; i <= n; ++i) {
            delete[] graph[i];
            delete[] domains[i].colors;
        }
        delete[] graph;
        delete[] graph_size;
        delete[] domains;
        delete[] color;
    }
    
    // 输出平均性能
    cout << "平均用时:" << sum_time/10 << "ms" << endl;
    return 0;
}
