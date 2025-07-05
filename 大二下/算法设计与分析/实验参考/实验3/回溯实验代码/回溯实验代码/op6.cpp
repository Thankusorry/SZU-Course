#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const int op = 0;  // 0为求一组解, 1为求所有解
int MAXC = 4;  // 颜色数
int n, m;
int maxd;  // 节点度数的最大值
vector<set<int>> edges;
vector<int> d;
vector<int> colors;
vector<set<int>> canUse;  // 节点可染的颜色的集合

void input() {
    string name; cin >> name;
    name = ".\\data\\" + name;
    freopen(name.c_str(), "r", stdin);

    cin >> n >> m >> MAXC;
    edges.resize(n + 1);
    d.resize(n + 1);
    colors.resize(n + 1);

    set<int> s;
    for (int i = 1; i <= MAXC; i++) s.insert(i);
    canUse = vector<set<int>>(n + 1, s);
    canUse[0].insert(-1);  // 保证canUse[0].size() > 其他的canUse[u].size()

    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;

        edges[u].insert(v), edges[v].insert(u);
        d[u]++, d[v]++;
    }

    for (int i = 1; i <= n; i++) maxd = max(maxd, d[i]);
}

bool check(int u, int color) {  // 检查节点u能否染color色
    for (int v = 1; v < u; v++)
        if (edges[u].count(v) && colors[v] == color) return false;
    return true;
}

ll dfs(int idx) {  // 已染色的节点数
    if (idx > n) return 1;
    
    // 贪心一个可染的颜色最少且度数最大的节点
    int u = 0;
    for (int i = 1; i <= n; i++) {
        if (!colors[i]) {
            if (canUse[i].size() < canUse[u].size() || (canUse[i].size() == canUse[u].size() && d[i] > d[u]))
                u = i; 
        }
    }

    vector<int> nums(MAXC + 1);  // nums[c]表示颜色c在节点u的邻居节点的可染颜色中出现的次数
    for (int v = 1; v <= n; v++) {
        if (edges[u].count(v)) {
            for (auto c : canUse[v])
                nums[c]++;
        }
    }
 
    vector<pair<int, int>> can;  // first为出现次数, second为颜色
    for (auto c : canUse[u]) 
        can.push_back({ nums[c], c });
    sort(all(can));

    ll res = 0;
    for (auto [_, c] : can) {
        // 染色, 并更新节点u及其邻居节点的canUse[]和cnt[]
        colors[u] = c;
        canUse[u].erase(c);
        vector<int> lims;  // 被限制的节点
        bool ok = true;  // 记录方案是否合法
        for (int v = 1; v <= n; v++) {
            if (edges[u].count(v) && !colors[v] && canUse[v].count(c)) {
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

void solve() {
    input();
    
    clock_t my_clock = clock();
    auto ans = dfs(1);  // 给第一个节点染色
    cout << "[n, m, MAXC, maxd] " << n << ' ' << m << ' ' << MAXC << ' ' << maxd << endl;
    int timeCost = clock() - my_clock;
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