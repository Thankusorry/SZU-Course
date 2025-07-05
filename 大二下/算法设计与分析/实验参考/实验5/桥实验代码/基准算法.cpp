#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;



void solve() {
    clock_t my_clock = clock();

    int n, m; cin >> n >> m;
    vector<set<int>> adj(n + 1);
    vector<pair<int, int>> edges(m);
    for (auto& [u, v] : edges) {
        cin >> u >> v;
        u++, v++;
        adj[u].insert(v), adj[v].insert(u);
    }

    auto count = [&]() {  // 求连通块个数
        vector<bool> vis(n + 1);

        auto bfs = [&](int st) {
            queue<int> que;
            que.push(st);

            while (que.size()) {
                auto u = que.front(); que.pop();
                vis[u] = true;
                for (auto v : adj[u])
                    if (!vis[v]) que.push(v);
            }
        };

        int cnt = 0;
        for (int u = 1; u <= n; u++) {
            if (!vis[u]) {
                cnt++;
                bfs(u);
            }
        }
        return cnt;
    };

    auto remove = [&](int u, int v) {
        adj[u].erase(v), adj[v].erase(u);
    };

    auto add = [&](int u, int v) {
        adj[u].insert(v), adj[v].insert(u);
    };

    int cnt = count();  // 原图连通块个数
    int ans = 0;  // 桥数
    for (auto [u, v] : edges) {
        remove(u, v);
        ans += count() != cnt;
        add(u, v);
    }
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