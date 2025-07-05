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

int cmp(double a, double b) {
    if (fabs(a - b) < eps) return 0;
    else return a < b ? -1 : 1;
}

int n;
vector<pii> points;
double minDis = 0x3f3f3f3f3f3f3f3f;  // 最短距离
pii ans;  // 取得最短距离的两点的编号

void input() {
    cin >> n;
    points.resize(n);
    
    string name = ".\\test\\in" + to_string(n) + ".txt";
    freopen(name.c_str(), "r", stdin);
    for (auto& [x, y] : points) cin >> x >> y;
}

double getDistance(int i, int j) {
    return hypot(points[i].x - points[j].x, points[i].y - points[j].y);
}

void bruteForce() {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dis = getDistance(i, j);
            if (cmp(dis, minDis) < 0) {
                minDis = dis;
                ans = { i, j };
            }
        }
    }

    cout << "Point " << '[' << points[ans.first].x << ", " << points[ans.first].y << "] and point [" 
         << points[ans.second].x << ", " << points[ans.second].y 
         << "] has the shortest distance of " << minDis << endl;
}

void solve() {
    input();
    
    clock_t my_clock = clock(); 
    bruteForce();
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