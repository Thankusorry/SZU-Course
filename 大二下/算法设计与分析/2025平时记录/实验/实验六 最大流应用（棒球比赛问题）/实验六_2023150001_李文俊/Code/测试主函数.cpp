#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9; // 定义无穷大常量

// 队伍数据结构
struct Team
{
    string name;         // 队伍名称
    int wins;            // 已获胜场次
    int losses;          // 已失败场次
    int remaining;       // 剩余比赛场次
    vector<int> against; // 与其他队伍的比赛场次
};

// 边数据结构（用于前向星图表示）
struct Edge
{
    int to;       // 边的终点
    int capacity; // 当前容量
    int next;     // 同一个起点的下一条边的索引
    int rev;      // 反向边在edge_pool中的索引
};

// --- 全局图结构（前向星表示）---
Edge *edge_pool; // 存放所有边的连续内存池
int *dist;       // ISAP算法的距离标号
int *gap;        // ISAP算法的GAP优化数组
int *level;      // Dinic算法的分层图层级
int *head;       // 头指针数组, head[u]存储u的第一条边的索引
int *iter;       // 当前弧优化所需的迭代器指针
int edge_count;  // 当前已添加的边的数量

// 初始化图结构
// 参数:
//   num_nodes: 节点总数
//   max_edges: 最大边数
void init_graph(int num_nodes, int max_edges)
{
    // 预分配足够大的内存
    edge_pool = new Edge[max_edges];
    head = new int[num_nodes];
    iter = new int[num_nodes];
    level = new int[num_nodes];
    dist = new int[num_nodes];
    gap = new int[num_nodes + 1]; // gap数组的大小要比节点数大1，因为距离可以达到n

    // 初始化头指针数组，-1表示没有出边
    std::fill(head, head + num_nodes, -1);
    edge_count = 0; // 边计数器清零
}

// 添加边的函数（前向星核心）
// 参数:
//   u: 起点
//   v: 终点
//   cap: 容量
void add_edge_ptr(int u, int v, int cap)
{
    // 添加正向边 u -> v
    edge_pool[edge_count] = {v, cap, head[u], edge_count + 1};
    head[u] = edge_count; // 更新头指针
    edge_count++;

    // 添加反向边 v -> u（容量为0）
    edge_pool[edge_count] = {u, 0, head[v], edge_count - 1};
    head[v] = edge_count; // 更新头指针
    edge_count++;
}

// 释放图内存
void free_graph()
{
    delete[] edge_pool;
    delete[] head;
    delete[] iter;
    delete[] level;
    delete[] dist;
    delete[] gap;
}

int main()
{
    // 1. 读取输入文件
    ifstream inputFile("teams50.txt");
    if (!inputFile)
    {
        cerr << "Error opening file" << endl;
        return 1;
    }

    int n; // 队伍数量
    inputFile >> n;

    vector<Team> teams(n); // 队伍数据数组
    for (int i = 0; i < n; ++i)
    {
        teams[i].against.resize(n); // 初始化对阵数组
        // 读取队伍数据：名称、胜场、负场、剩余场次
        inputFile >> teams[i].name >> teams[i].wins >> teams[i].losses >> teams[i].remaining;
        // 读取与其他队伍的比赛场次
        for (int j = 0; j < n; ++j)
        {
            inputFile >> teams[i].against[j];
        }
    }
    inputFile.close();

    double sum = 0; // 总时间统计

    // 2. 检查每个队伍是否被淘汰
    for (int i = 0; i < n; ++i)
    {
        // 计算队伍i的最大可能胜场
        int max_possible_wins = teams[i].wins + teams[i].remaining;
        bool eliminated = false;    // 是否被淘汰
        vector<string> certificate; // 淘汰证明队伍列表

        // 3. 简单淘汰检查：是否存在其他队伍胜场已超过i的最大可能胜场
        for (int j = 0; j < n; ++j)
        {
            if (i == j)
                continue; // 跳过自身

            if (max_possible_wins < teams[j].wins)
            {
                eliminated = true;
                certificate.push_back(teams[j].name);
                break; // 只需一个队伍即可证明淘汰
            }
        }

        // 如果已被简单淘汰，输出结果并继续下一个队伍
        if (eliminated)
        {
            cout << teams[i].name << " is eliminated by the subset R = { ";
            for (size_t k = 0; k < certificate.size(); ++k)
            {
                cout << certificate[k] << (k == certificate.size() - 1 ? "" : " ");
            }
            cout << " }" << endl;
            continue;
        }

        // 4. 构建流网络进行复杂淘汰检查

        // 计算比赛节点数量（排除队伍i和队伍i参与的比赛）
        int num_games_nodes = 0;
        for (int j = 0; j < n; ++j)
        {
            for (int k = j + 1; k < n; ++k)
            {
                // 排除队伍i参与的比赛
                if (j != i && k != i && teams[j].against[k] > 0)
                {
                    num_games_nodes++;
                }
            }
        }

        // 节点总数 = 源点(s) + 汇点(t) + 比赛节点 + 队伍节点
        int num_teams_nodes = n; // 队伍节点数（包括所有队伍）
        int total_nodes = 2 + num_teams_nodes + num_games_nodes;

        // 估计最大边数：每个比赛节点有3条边（源点->比赛，比赛->队伍A，比赛->队伍B）
        // 加上队伍节点到汇点的边，乘以2（正反向边）
        int max_edges = (num_games_nodes * 3 + n) * 2;
        max_edges += 10; // 添加一些缓冲

        // 初始化图结构
        init_graph(total_nodes, max_edges);

        // 设置源点和汇点
        int s = 0, t = total_nodes - 1;
        int game_node_idx = 1;    // 比赛节点从1开始编号
        int total_games_flow = 0; // 源点流出的总流量（所有比赛场次总和）

        // 5. 构建流网络
        // 5.1 添加源点到比赛节点的边
        for (int j = 0; j < n; ++j)
        {
            for (int k = j + 1; k < n; ++k)
            {
                // 排除队伍i参与的比赛
                if (j == i || k == i)
                    continue;

                if (teams[j].against[k] > 0) // 如果有比赛
                {
                    // 累加比赛场次
                    total_games_flow += teams[j].against[k];

                    // 添加源点到比赛节点的边，容量为比赛场次
                    add_edge_ptr(s, game_node_idx, teams[j].against[k]);

                    // 计算队伍j和k对应的节点索引
                    int team_node_j = 1 + num_games_nodes + j;
                    int team_node_k = 1 + num_games_nodes + k;

                    // 添加比赛节点到队伍节点的边，容量为无穷大
                    add_edge_ptr(game_node_idx, team_node_j, INF);
                    add_edge_ptr(game_node_idx, team_node_k, INF);

                    game_node_idx++; // 下一个比赛节点
                }
            }
        }

        // 5.2 添加队伍节点到汇点的边
        for (int j = 0; j < n; ++j)
        {
            if (i == j)
                continue; // 跳过队伍i

            int team_node_idx = 1 + num_games_nodes + j;

            // 容量 = 队伍i的最大可能胜场 - 队伍j的当前胜场
            int cap = max_possible_wins - teams[j].wins;
            if (cap < 0)
                cap = 0; // 确保容量非负

            add_edge_ptr(team_node_idx, t, cap);
        }

        // 6. 计算最大流并测量时间
        auto start = chrono::high_resolution_clock::now();

        // 调用ISAP算法计算最大流
        int max_flow = isap(s, t, total_nodes);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> elapsed = end - start;
        sum += elapsed.count(); // 累加
