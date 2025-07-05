#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

struct RevocableDSU {
    int n;
    vector<int> fa;
    vector<int> siz;
    vector<pair<int&, int>> hisSiz;
    vector<pair<int&, int>> hisFa;
    int component;

    RevocableDSU() {}
    RevocableDSU(int _n) :n(_n), component(n) {
        fa.resize(n + 1);
        iota(all(fa), 0);
        siz = vector<int>(n + 1, 1);
    }

    int find(int x) {  // 无路径压缩
        while (x != fa[x]) x = fa[x];
        return x;
    }

    bool merge(int x, int y) {  // 按秩合并, 返回合并是否成功
        if ((x = find(x)) == (y = find(y))) return false;
        
        component--;
        if (siz[x] < siz[y]) swap(x, y);
        hisSiz.push_back({ siz[x], siz[x] });
        siz[x] += siz[y];
        hisFa.push_back({ fa[y], fa[y] });
        fa[y] = x;
        return true;
    }

    int getSize(int x) {
        return siz[find(x)];
    }

    bool check(int x, int y) {
        return find(x) == find(y);
    }

    int getVersion() {  // 当前版本的版本号
        return hisFa.size();
    }

    void rollback(int ver) {  // 回退到第ver个版本
        while (hisFa.size() > ver) {
            component++;
            hisFa.back().first = hisFa.back().second;
            hisFa.pop_back();
            hisSiz.back().first = hisSiz.back().second;
            hisSiz.pop_back();
        }
    }
};

struct SegmentTreeDivide {
    int n, m;  // 节点数、询问数
    RevocableDSU dsu;
    struct Node {
        vector<pair<int, int>> edges;  // 该时刻存在的边
    };
    vector<Node> SegT;
    vector<int> ans;

    SegmentTreeDivide(int _n, int _m) :n(_n), m(_m) {
        dsu = RevocableDSU(n);
        SegT.resize(m + 5 << 2);
        ans.resize(m + 1);
    }

    void insert(int u, int l, int r, int L, int R, pair<int, int> edge) {
        if (L <= l && r <= R) {
            SegT[u].edges.push_back(edge);
            return;
        }

        int mid = l + r >> 1;
        if (L <= mid) insert(u << 1, l, mid, L, R, edge);
        if (R > mid) insert(u << 1 | 1, mid + 1, r, L, R, edge);
    }

    void insert(int L, int R, pair<int, int> edge) {
        insert(1, 1, m, L, R, edge);
    }

    void query(int rt, int l, int r) {
        int ver = dsu.getVersion();

        for (auto [u, v] : SegT[rt].edges)
            dsu.merge(u, v);

        if (l == r) ans[l] = dsu.component;
        else {
            int mid = l + r >> 1;
            query(rt << 1, l, mid), query(rt << 1 | 1, mid + 1, r);
        }

        dsu.rollback(ver);
    }

    void query() {
        query(1, 1, m);
    }
};

void solve() {
    clock_t my_clock = clock();

    int n, m; cin >> n >> m;
    vector<pair<int, int>> edges(m + 1);
    for (int i = 1; i <= m; i++) {
        auto& [u, v] = edges[i];
        cin >> u >> v;

        u++, v++;
        if (u > v) swap(u, v);
    }

    if (!m) return;  // m = 0时会RE

    int MAXM = m * 4 + 1;
    SegmentTreeDivide solver(n, MAXM);
    map<pair<int, int>, int> last;  // edge, time

    int tim = 1;  // 时间戳

    // 将所有边加入图中
    for (int i = 1; i <= m; i++) {
        auto [u, v] = edges[i];
        last[{ u, v }] = tim++;
    }

    vector<int> ques;  // 询问
    ques.push_back(tim++);

    for (int i = 1; i <= m; i++) {
        auto [u, v] = edges[i];

        // 删边
        solver.insert(last[{ u, v }], (tim++) - 1, { u, v });
        last[{ u, v }] = -1;

        // 询问连通块数
        ques.push_back(tim++);

        // 加边
        last[{ u, v }] = tim++;
    }

    for (auto [que, idx] : last)
        if (idx >= 1) solver.insert(idx, MAXM, que);

    solver.query();

    int ans = 0;  // 桥数
    for (int i = 1; i < ques.size(); i++)
        ans += solver.ans[ques[i]] != solver.ans[ques[0]];
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