#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
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
#define double long double
const double eps = 1e-8;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
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

#define x first
#define y second
const int MAXN = 205;
int n, m, k;
char maze[MAXN][MAXN];
pii s, t;  // 起点、终点
int gem;  // 宝石数
pii doors[MAXN];  // 传送门
int cnt;  // 传送门数
bool vis[MAXN][MAXN][MAXN];  // x,y,state
struct Node {
    int x, y;
    int step;
    int state;  // 当前收集到的宝石的状态
};

void init() {
    gem = cnt = 0;
    memset(vis, false, so(vis));
}

bool check(int state) {  // 检查是否已收集够宝石
    int res = 0;
    for (int i = 0; i < 5; i++) res += state >> i & 1;
    return res >= k;
}

int bfs() {
    queue<Node> que;
    que.push({ s.x, s.y, 0, 0 });
    vis[s.x][s.y][0] = true;

    while (que.size()) {
        auto tmp = que.front(); que.pop();
        for (int i = 0; i < 4; i++) {
            int curx = tmp.x + dx[i], cury = tmp.y + dy[i];
            int step = tmp.step, state = tmp.state;
            if (curx == t.x && cury == t.y && check(state)) return step + 1;
            if (curx < 1 || curx > n || cury < 1 || cury > m) continue;
            if (vis[curx][cury][state] || maze[curx][cury] == '#') continue;

            vis[curx][cury][state] = true;
            if (maze[curx][cury] == '.') que.push({ curx,cury,step + 1,state });
            else if (isdigit(maze[curx][cury])) {  // 宝石
                state = state | (1 << maze[curx][cury] - '0');
                que.push({ curx,cury,step + 1,state });
            }
            else if (maze[curx][cury] == '$') {  // 传送门
                que.push({ curx,cury,step + 1,state });  // 不跳转
                
                for (int j = 0; j < cnt; j++) {  // 枚举其他传送门
                    curx = doors[j].x, cury = doors[j].y;
                    if (!vis[curx][cury][state]) {
                        vis[curx][cury][state] = true;
                        que.push({ curx,cury,step + 1,state });
                    }
                }
            }
        }
    }

    return -1;
}

void solve() {
    init();

    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> maze[i][j];
            if (maze[i][j] == 'S') {
                s = { i,j };
                maze[i][j] = '.';
            }
            else if (maze[i][j] == 'E') {
                t = { i,j };
                maze[i][j] = '.';
            }
            else if (maze[i][j] == '$') doors[cnt++] = { i,j };
            else if (isdigit(maze[i][j])) gem++;
        }
    }

    if (gem < k) {
        cout << "oop!" << endl;
        return;
    }

    int ans = bfs();
    cout << (~ans ? to_string(ans) : "oop!") << endl;
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