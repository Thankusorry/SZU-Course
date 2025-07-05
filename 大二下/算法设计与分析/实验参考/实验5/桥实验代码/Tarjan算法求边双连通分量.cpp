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

    DSU() {}
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

struct TarjanEDCC {
    int n, m;
    vector<int> head, edge, nxt;
    int idx;
    vector<int> dfn;  // 节点的DFS序
    vector<int> low;  // 节点能追溯到的最小时间戳
    int tim;  // 时间戳
    // stack<int> stk;
    vector<int> stk;
    int top;
    vector<int> id;  // 节点所在的e-DCC的编号
    int edccCnt;  // e-DCC的个数
    vector<bool> isBridge;
    DSU dsu;

    TarjanEDCC(int _n, int _m) :n(_n), m(_m * 2), idx(0), tim(0), edccCnt(0) {
        head = vector<int>(n + 5, -1);
        edge.resize(m + 5), nxt.resize(m + 5);
        dfn.resize(n + 5), low.resize(n + 5);
        id.resize(n + 5), isBridge.assign(m + 5, false);

        stk.resize(n + 5);
        top = 0;
        dsu = DSU(n);
    }

    void add(int u, int v) {
        edge[idx] = v, nxt[idx] = head[u], head[u] = idx++;
    }

    void addEdge(int u, int v) {
        add(u, v), add(v, u);
        dsu.merge(u, v);
    }

    void tarjan(int u, int pre) {  // 当前节点、前驱边
        dfn[u] = low[u] = ++tim;
        // stk.push(u);
        stk[++top] = u;

        for (int i = head[u]; ~i; i = nxt[i]) {
            int v = edge[i];
            if (!dfn[v]) {
                tarjan(v, i);
                low[u] = min(low[u], low[v]);
                if (dfn[u] < low[v])
                    isBridge[i] = isBridge[i ^ 1] = true;
            }
            else if (i != (pre ^ 1))  // 非反向边
                low[u] = min(low[u], dfn[v]);
        }

        if (dfn[u] == low[u]) {  // u是所在的e-DCC的深度最小的节点
            edccCnt++;
            int v;
            do {
                // v = stk.top(); stk.pop();
                v = stk[top--];
                id[v] = edccCnt;
            } while (v != u);
        }
    }

    void work() {
        vector<int> components;  // 每个连通块的代表元素
        for (int u = 1; u <= n; u++) {
            if (dsu.fa[u] == u)
                components.push_back(u);
        }

        for (int i = 1; i < components.size(); i++)
            addEdge(components[0], components[i]);
        
        tarjan(1, -1);

        int res = 0;
        for (int i = 0; i < idx; i += 2) 
            res += isBridge[i];
        cout << "ans = " << res - ((int)components.size() - 1) << endl;
    }
};

void solve() {
    clock_t my_clock = clock();
    
    int n, m; cin >> n >> m;

    TarjanEDCC solver(n, m);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;

        u++, v++;
        solver.addEdge(u, v);
    }

    solver.work();

    cout << "[n, m] " << n << ' ' << m << endl;
    cout << clock() - my_clock << " ms." << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);

    string name; cin >> name;
    freopen(name.c_str(), "r", stdin);

    solve();
    return 0;
}