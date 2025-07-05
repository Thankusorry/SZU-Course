#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

typedef pair<int, int> pii;
#define x first
#define y second

mt19937 rnd(time(0));

const double eps = 1e-8;

int doubleCmp(double a, double b) {
    if (fabs(a - b) < eps) return 0;
    else return a < b ? -1 : 1;
}

int n;
vector<pii> points;
double minDis = 0x3f3f3f3f3f3f3f3f;  // 最短距离
pair<pii, pii> ans;  // 取得最短距离的两点

void input() {
    cin >> n;
    points.resize(n);
    
    string name = ".\\test\\in" + to_string(n) + ".txt";
    freopen(name.c_str(), "r", stdin);
    for (auto& [x, y] : points) cin >> x >> y;
}

struct cmp {
    bool operator()(const pii& A, const pii& B)const {
        return A.y < B.y;
    }
};

double getDistance(const pii& A, const pii& B) {  // dis(A, B)
    return hypot(A.x - B.x, A.y - B.y);
}

void multisetAlgorithm() {
    sort(all(points), [&](const pii& A, const pii& B) {
        return A.x != B.x ? A.x < B.x : A.y < B.y;
    });

    multiset<pii, cmp> s;
    for (int i = 0, j = 0; i < n; i++) {
        // 删除集合中x_i - x_j >= ans的点j
        while (j < i && points[i].x - points[j].x >= minDis) 
            s.erase(s.find(points[j++]));

        // 暴力更新集合中满足|x_i - x_j| < ans的点j的答案
        for (auto it = s.lower_bound(pii(points[i].x, points[i].y - minDis)); 
            it != s.end() && it->y - points[i].y < minDis; it++) {
                double dis = getDistance(*it, points[i]);
                if (doubleCmp(dis, minDis) < 0) {
                    minDis = dis;
                    ans = { *it, points[i] };
                }
            }
        
        s.insert(points[i]);
    }

    cout << "Point " << '[' << ans.first.x << ", " << ans.first.y << "] and point [" 
         << ans.second.x << ", " << ans.second.y 
         << "] has the shortest distance of " << minDis << endl;
}

void solve() {
    input();
    
    clock_t my_clock = clock(); 
    multisetAlgorithm();
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