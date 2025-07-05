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

void input() {
    string s; cin >> s;
    s = ".\\data\\" + s;
    freopen(s.c_str(), "r", stdin);

    cin >> n >> m >> MAXC;
    edges = vector<vector<bool>>(n + 1, vector<bool>(n + 1));
    d.resize(n + 1);
    colors.resize(n + 1);

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

ll dfs(int idx) {  // 给节点idx染色
    if (idx > n) return 1;
 
    ll res = 0;
    for (int c = 1; c <= MAXC; c++) {
        if (check(idx, c)) {
            colors[idx] = c;
            if (op) res += dfs(idx + 1);
            else if (dfs(idx + 1)) return 1;
            colors[idx] = 0;
        }
    }
    return res;
}

void solve() {
    input();
    
    clock_t my_clock = clock(); 
    auto ans = dfs(1);  // 给节点1染色
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