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

const int MAXN = 1e5;
namespace HashTable {
    const int MOD = 11;
    int len;  // 表长
    int ha[MAXN];

    void init() {
        for (int i = 0; i < len; i++) ha[i] = -1;
    }

    void insert(int x) {
        int pos = x % MOD;
        for (int cur = 0;; cur++) {
            for (int i = 0; i < 2; i++) {
                int tmp = ((pos + (i ? -1 : 1) * cur * cur) % len + len) % len;
                if (ha[tmp] == -1) {
                    ha[tmp] = x;
                    return;
                }
            }
        }
    }

    pii find(int x) {  // 返回查找次数、查找成功的位置
        int pos = x % MOD;
        int cnt = 1;
        for (int cur = 0;; cur++) {
            for (int i = 0; i < 2; i++) {
                int tmp = ((pos + (i ? -1 : 1) * cur * cur) % len + len) % len;
                if (cur) cnt++;  // cur=0时只需一次

                if (ha[tmp] == -1) return{ cnt,-1 };
                else if (ha[tmp] == x) return { cnt,tmp + 1 };
            }

            if (cur > sqrt(len)) return { cnt,-1 };
        }
    }

    void print() {
        for (int i = 0; i < len; i++)
            cout << (~ha[i] ? to_string(ha[i]) : "NULL") << " \n"[i == len - 1];
    }
}
using namespace HashTable;

void solve() {
    int n; cin >> len >> n;

    init();
    while (n--) {
        int x; cin >> x;
        insert(x);
    }
    print();

    CaseT{
        int x; cin >> x;
        auto [u, v] = find(x);
        if (~v) cout << 1 << ' ' << u << ' ' << v << endl;
        else cout << 0 << ' ' << u << endl;
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