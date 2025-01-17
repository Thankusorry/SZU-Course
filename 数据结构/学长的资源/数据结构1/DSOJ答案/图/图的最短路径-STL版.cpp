#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<complex>
#include<array>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<list>
#include<unordered_set>
#include<unordered_map>
#include<bitset>
#include<valarray>
#include<sstream>
#include<functional>
#include<cassert>
#include<random>
#include<numeric>
using namespace std;
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<bool> vb;
typedef queue<int> qi;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, double> pdd;
typedef tuple<int, int, int> tiii;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef queue<pii> qii;
typedef complex<double> cp;
#define umap unordered_map
#define uset unordered_set
#define pque priority_queue
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define npt nullptr
#define so sizeof
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define hashset_finetune(p) (p).reserve(1024); (p).max_load_factor(0.25);  // 哈希表防卡
const double eps = 1e-7;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
// const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
// ----------------------------------------------------------------
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll lcm(ll a, ll b) { return a * b / gcd(a, b); }
ll lowbit(ll x) { return x & (-x); }
int cmp(double a, double b) {
    if (fabs(a - b) < eps) return 0;
    else if (a > b) return 1;
    else return -1;
}
int sgn(double x) {
    if (fabs(x) < eps) return 0;
    else if (x > 0) return 1;
    else return -1;
}

template<typename T>
void read(T& x) {
    x = 0;
    T sgn = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') sgn = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + (ch & 15);
        ch = getchar();
    }
    x *= sgn;
}

template<typename T>
void write(T x) {
    if (x < 0) {
        putchar('-');
        x *= -1;
    }
    if (x < 10) putchar(x + 48);
    else write(x / 10), putchar(x % 10 + 48);
}

ll qpow(ll a, ll k, ll MOD) {
    ll res = 1;
    while (k) {
        if (k & 1) res = res * a % MOD;
        k >>= 1;
        a = a * a % MOD;
    }
    return res;
}

ll qpow(ll a, ll k) {  // 注意可能爆ll
    ll res = 1;
    while (k) {
        if (k & 1) res = res * a;
        k >>= 1;
        a *= a;
    }
    return res;
}
// ----------------------------------------------------------------

const int MAXN = 1005;
map<int, string> mp;
map<string, int> pm;
int idx = 0;
int n;
int w[MAXN][MAXN];  // 邻接矩阵
int dis[MAXN];  // 每个节点到起点的距离
bool state[MAXN];  // 记录当前每个节点是否已确定最短路径
int pre[MAXN];  // 记录每个节点的前驱节点

int dijkstra(int S, int T) {  // 求起点S到终点T的最短路
    memset(dis, INF, sizeof(dis));
    memset(state, false, so(state));
    dis[S] = 0, pre[S] = S;

    for (int i = 0; i < n - 1; i++) {  // 枚举每个节点
        int t = -1, j;
        for (j = 1; j <= n; j++) {  // 枚举不在state中且距离最近的点t
            if (!state[j] && (t == -1 || dis[t] > dis[j]))  // 注意t==-1时直接赋值
                t = j;
        }

        if (j == T) break;  // 已找到1->n的最短距离即可返回

        state[t] = true;  // t到起点的最短距离已确定
        for (j = 1; j <= n; j++) {  // 用t的所有出边更新各节点到起点的最短距离
            if (dis[t] + w[t][j] < dis[j]) {
                dis[j] = dis[t] + w[t][j];
                pre[j] = t;
            }
        }
    }
    return dis[T] == INF ? -1 : dis[T];  // 节点T的距离未被更新,则S与T不连通
}

void solve() {
    idx = 0;
    mp.clear();

    cin >> n;
    for (int i = 1; i <= n; i++) {
        string s; cin >> s;
        mp[i] = s, pm[s] = i;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n;j++) {
            cin >> w[i][j];
            if (!w[i][j]) w[i][j] = INF;
        }
    }

    string s; cin >> s;
    int S = pm[s];
    for (int i = 1; i <= n; i++) {
        if (i != S) {
            int ans = dijkstra(S, i);
            if (~ans) {
                cout << mp[S] << '-' << mp[i] << '-' << ans << "----[";
                vector<string> res;
                int cur = i;
                while (cur != S) {
                    res.push_back(mp[cur]);
                    cur = pre[cur];
                }
                res.push_back(mp[S]);

                reverse(all(res));
                for (auto s : res) cout << s << ' ';
                cout << ']' << endl;
            }
            else cout << mp[S] << '-' << mp[i] << "--1" << endl;
        }
    }
}

int main() {
    IOS;
#ifndef ONLINE_JUDGE
    clock_t my_clock = clock();
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // ----------------------------------------------------------------	
    // init();
    CaseT  // 单测时注释掉该行
    solve();
    // ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
    cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
    return 0;
}