#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const int op = 1;  // 0为求一组解, 1为求所有解
int MAXC = 4;  // 颜色数
int n, m;
int maxd;  // 节点度数的最大值
vector<vector<bool>> edges;
vector<int> d;
vector<int> colors;
vector<set<int>> canUse;  // 节点可染的颜色的集合

void input() {
    string name; cin >> name;
    name = ".\\data\\" + name;
    freopen(name.c_str(), "r", stdin);

    cin >> n >> m >> MAXC;
    edges = vector<vector<bool>>(n + 1, vector<bool>(n + 1));
    d.resize(n + 1);
    colors.resize(n + 1);

    set<int> s;
    for (int i = 1; i <= MAXC; i++) s.insert(i);
    canUse = vector<set<int>>(n + 1, s);
    canUse[0].insert(-1);  // 保证canUse[0].size() > 其他的canUse[u].size()

    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;

        edges[u][v] = edges[v][u] = true;
        d[u]++, d[v]++;
    }

    for (int i = 1; i <= n; i++) maxd = max(maxd, d[i]);
}

bool check(int u, int color) {  // 检查节点u能否染color色
    for (int v = 1; v < u; v++)
        if (edges[u][v] && colors[v] == color) return false;
    return true;
}

ll dfs(int idx) {  // 已染色的节点数, 与op3.cpp相同
    if (idx > n) return 1;
    
    // 贪心一个可染的颜色最少且度数最大的节点
    int u = 0;
    for (int i = 1; i <= n; i++) {
        if (!colors[i]) {
            if (canUse[i].size() < canUse[u].size() || (canUse[i].size() == canUse[u].size() && d[i] > d[u]))
                u = i; 
        }
    }
 
    ll res = 0;
    auto can = canUse[u];  // 一边遍历一边修改是ub
    for (auto c : can) {
        // 染色, 并更新节点u及其邻居节点的canUse[]和cnt[]
        colors[u] = c;
        canUse[u].erase(c);
        vector<int> lims;  // 被限制的节点
        bool ok = true;  // 记录方案是否合法
        for (int v = 1; v <= n; v++) {
            if (edges[u][v] && !colors[v] && canUse[v].count(c)) {
                canUse[v].erase(c);
                if (canUse[v].empty()) {
                    ok = false;
                    // break;  // 此处不能break, 否则找不到解
                }
                lims.push_back(v);
            }
        }

        if (ok) {  // 方案合法才递归
            if (op) res += dfs(idx + 1);
            else if (dfs(idx + 1)) return 1;
        }
        
        // 回溯, 并更新节点u及其邻居节点的canUse[]和cnt[]
        colors[u] = 0;
        canUse[u].insert(c);
        for (auto v : lims) canUse[v].insert(c);
    }
    return res;
}

vector<int> modify(int u, int c) {  // 给节点u染c色, 返回可染颜色数改变的节点
    colors[u] = c;
    canUse[u].erase(c);
    vector<int> res;
    for (int v = 1; v <= n; v++) {
        if (edges[u][v] && !colors[v] && canUse[v].count(c)) {
            canUse[v].erase(c);
            res.push_back(v);
        }
    }
    return res;
}

void recover(int u, vector<int> lims, int c) {  // 撤销节点u的染色, 并恢复受影响的节点的可染颜色数
    colors[u] = 0;
    canUse[u].insert(c);
    for (auto v : lims) canUse[v].insert(c);
}

ll work() {
    ll res = 0;
    if (n == 1) res = MAXC;
    else if (n == 2) res = MAXC * (MAXC - 1);
    else {
        modify(1, 1);  // 给节点1染1色

        // 贪心一个节点1的度数最大的邻居节点, 必存在
        int u = 0;
        for (int i = 1; i <= n; i++) {
            if (edges[1][i] && d[i] > d[u])
                u = i;
        }

        modify(u, 2);  // 给节点u染2色

        // 贪心一个节点1和节点u公共的、度数最大的邻居节点, 未必存在
        int v = 0;
        for (int i = 1; i <= n; i++) {
            if (edges[1][i] && edges[u][i] && d[i] > d[v])
                v = i;
        }

        if (v) {  // 存在节点v, 则为环
            modify(v, 3);
            res += MAXC * (MAXC - 1) * (MAXC - 2) * dfs(4);
        }
        else {  // 不存在节点v, 则为链
            // 贪心一个与节点1或节点u相连的且度数最大的邻居节点
            for (int i = 1; i <= n; i++) {
                if (i != 1 && i != u && (edges[1][i] || edges[u][i]) && d[i] > d[v])
                    v = i;
            }

            // 首尾同色
            auto lims = modify(v, 1);  // 给节点v染1色
            res += MAXC * (MAXC - 1) * dfs(4);

            // 首尾异色
            recover(v, lims, 1);  // 撤销节点v的染色
            modify(v, 3);  // 给节点v染3色
            res += MAXC * (MAXC - 1) * (MAXC - 2) * dfs(4);
        }
    }
    return res;
}

void solve() {
    input();
    
    clock_t my_clock = clock();
    auto ans = work();
    int timeCost = clock() - my_clock;
    cout << "[n, m, MAXC, maxd] " << n << ' ' << m << ' ' << MAXC << ' ' << maxd << endl;
    cout << "ans = " << ans << endl;
    cout << "Time cost: " << (int)timeCost << " ms." << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    // init();
    // CaseT
    solve();
    return 0;
}