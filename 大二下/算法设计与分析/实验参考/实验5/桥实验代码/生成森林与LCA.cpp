#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

struct DSU {
    int n;
    vector<int> fa;

    DSU(int _n) :n(_n) {
        fa.resize(n + 1);
        iota(all(fa), 0);
    }
    
    int find(int x) {
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }

    bool merge(int x, int y) {
        if ((x = find(x)) == (y = find(y))) return false;

        fa[x] = y;
        return true;
    }

    bool check(int x, int y) {
        return find(x) == find(y);
    }

    int count() {
        int cnt = 0;
        for (int i = 1; i <= n; i++) cnt += fa[i] == i;
        return cnt;
    }
};

void solve() {
    clock_t my_clock = clock();

    int n, m; cin >> n >> m;
    vector<pair<int, int>> eds(m + 1);  // u, v
    vector<vector<pair<int, int>>> edges(n + 1);  // v, idx
    for (int i = 1; i <= m; i++) {
        auto& [u, v] = eds[i];
        cin >> u >> v;

        u++, v++;
        edges[u].push_back({ v, i }), edges[v].push_back({ u, i });
    }

    vector<int> vis(n + 1);  // 0表示未被遍历, 1表示正在被遍历, 2表示已被遍历
    vector<pair<int, int>> pre(n + 1);  // 节点在DFS树上的前驱和连接前驱的边
    vector<int> depth(n + 1);  // 节点在DFS树上的深度
    vector<bool> spEdge(m + 1);  // 边是否在生成树上
    
    function<void(int, int)> dfs = [&](int u, int fa) {
        vis[u] = 1, depth[u] = depth[fa] + 1;
        for (auto [v, idx] : edges[u]) {
            if (v != fa && !vis[v]) {
                spEdge[idx] = true;
                pre[v] = { u, idx };
                dfs(v, u);
            }
        }
        vis[u] = 2;
    };
    
    for (int u = 1; u <= n; u++)
        if (!vis[u]) dfs(u, 0); 

    vector<bool> loopEdge(m + 1);  // 边是否为环边
    DSU dsu(n);
    for (int i = 1; i <= m; i++) {
        if (spEdge[i]) dsu.merge(eds[i].first, eds[i].second);
        else loopEdge[i] = true;
    }

    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);

        while (depth[u] != depth[v]) {
            loopEdge[pre[u].second] = true;
            u = pre[u].first;
        }

        if (u == v) return;

        while (pre[u].first != pre[v].first) {
            loopEdge[pre[u].second] = loopEdge[pre[v].second] = true;
            u = pre[u].first, v = pre[v].first;
        }
    };

    for (int i = 1; i <= m; i++) {
        if (!spEdge[i]) {
            auto [u, v] = eds[i];
            if (dsu.check(u, v)) 
                lca(u, v);
        }
    }

    int ans = m;
    for (int i = 1; i <= m; i++) 
        ans -= loopEdge[i];
    cout << "ans = " << ans << endl;

    cout << "[n, m] " << n << ' ' << m << endl;
    cout << clock() - my_clock << " ms." << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);

    string name; cin >> name;
    name = ".\\data\\" + name;
    freopen(name.c_str(), "r", stdin);

    solve();
    return 0;
}