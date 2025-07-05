#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const double eps = 1e-8;

int cmp(double a, double b) {
    if (fabs(a - b) < eps) return 0;
    else return a < b ? -1 : 1;
}

typedef pair<int, int> pii;
#define x first
#define y second

mt19937 rnd(time(0));
const ll INFF = 0x3f3f3f3f3f3f3f3f;
const int d[] = { 0, -1, 1 };  // 偏移量

int n;
vector<pii> points;
unordered_map<ll, int> cnt;  // 哈希值相同的点数
unordered_map<ll, vector<pii>> mp;  // 哈希值对应的点

double minDis = 0x3f3f3f3f3f3f3f3f;  // 最短距离
pair<pii, pii> ans;  // 取得最短距离的两点

void input() {
    cin >> n;
    points.resize(n);
    
    string name = ".\\test\\in" + to_string(n) + ".txt";
    freopen(name.c_str(), "r", stdin);
    for (auto& [x, y] : points) cin >> x >> y;
}

double getDistance(const pii& A, const pii& B) {
    return ((ll)A.x - B.x) * (A.x - B.x) + ((ll)A.y - B.y) * (A.y - B.y);
}

ll getHash(ll x, ll y) {
    return x << 30 ^ y;
}

bool check(ll x, ll y) {
    if (cnt[getHash(x, y)] > 1) return true;
    for (int i = 0; i < 3; i++) {  // 枚举x坐标的偏移量
        for (int j = 0; j < 3; j++)  // 枚举y坐标的偏移量
            if ((i || j) && cnt.count(getHash(x + d[i], y + d[j]))) return true;
    }
    return false;
}

double cal(pii u, ll x, ll y) {
    double res = minDis;
    for (int i = 0; i < 2; i++) {  // 枚举x坐标的偏移量, 注意i只能取0, 1
        for (int j = 0; j < 3; j++) {  // 枚举y坐标的偏移量
            ll ha = getHash(x + d[i], y + d[j]);
            for (auto v : mp[ha]) {
                if (u != v) {
                    double dis = getDistance(u, v);
                    if (cmp(dis, res) < 0) {
                        minDis = res = dis;
                        ans = { u, v };
                    }
                }
            }
        }
    }
    return res;
}

void linear() {
    shuffle(all(points), rnd);

    vector<pii> a = points;
    double d;  // 最短距离的平方
    while (a.size() > 1) {
        d = INFF;
        for (int i = 1; i < a.size(); i++)
            d = min(d, getDistance(a[i], a[0]));
        if (d < eps) break;

        double l = sqrt(d) / 3;
        for (auto [x, y] : a)  // 将a[]中的点加入哈希表
            cnt[getHash(x / l, y / l)]++;

        vector<pii> tmp;  // 网格中的非孤立点
        for (auto [x, y] : a)
            if (check(x / l, y / l)) tmp.push_back({ x, y });
            
        ans = { a[0], a[1] };

        a = tmp;
        cnt.clear();
    }

    double dd = sqrt(d);
    minDis = d;
    if (minDis > eps) {
        for (auto [x, y] : points) 
            mp[getHash(x / dd, y / dd)].push_back({ x, y });
        
        for (auto [x, y] : points) 
            cal({ x, y }, x / dd, y / dd);
    }

    cout << "Point " << '[' << ans.first.x << ", " << ans.first.y << "] and point [" 
         << ans.second.x << ", " << ans.second.y 
         << "] has the shortest distance of " << sqrt(getDistance(ans.first, ans.second)) << endl;
}

void solve() {
    input();

    clock_t my_clock = clock(); 
    linear();
    double timeCost = clock() - my_clock;
    cout << "n = " << n << " costs " << fixed << setprecision(2) << timeCost << " ms." << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    // init();
    // CaseT
    solve();
    return 0;
}