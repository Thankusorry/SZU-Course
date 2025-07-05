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

    int count() {
        int cnt = 0;
        for (int i = 1; i <= n; i++) cnt += fa[i] == i;
        return cnt;
    }
};

void solve() {
    clock_t my_clock = clock();
    
    int n, m; cin >> n >> m;
    vector<pair<int, int>> edges(m);
    DSU dsu(n);
    for (auto& [u, v] : edges) {
        cin >> u >> v;
        u++, v++;
        dsu.merge(u, v);
    }

    int cnt = dsu.count();  // 原图的连通块数
    int ans = 0;  // 桥数
    for (int i = 0; i < m; i++) {  // 删除的边
        DSU tmp(n);
        for (int j = 0; j < m; j++) {
            if (j != i) 
                tmp.merge(edges[j].first, edges[j].second);
        }
        ans += tmp.count() != cnt;
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