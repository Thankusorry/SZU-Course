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
pair<pii, pii> ans;  // 取得最短距离的两点

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

double divide(int l, int r) {  // 合并: O(n log n)
    if (l == r) return INF;  // 只包含一个点
    else if (l + 1 == r) {  // 只包含2个点
        double dis = getDistance(l, r);
        if (dis < minDis) {
            minDis = dis;
            ans = { points[l], points[r] };
        }
        return dis;
    }

    int mid = l + r >> 1;
    double dis1 = divide(l, mid), dis2 = divide(mid + 1, r);

    double dis = min(dis1, dis2);    

    vector<int> tmp;  // 在区域[points[mid].x - dis, points[mid].x + dis]中的点
    for (int i = l; i <= r; i++) {
        if (points[i].x - points[mid].x <= dis)
            tmp.push_back(i);
    }
    sort(all(tmp), [&](const int& i, const int& j) {
        return points[i].y < points[j].y;
    });

    int siz = tmp.size();  // siz <= 6
    pair<pii, pii> res;
    for (int i = 0; i < siz; i++) {
        for (int j = i + 1; j < siz && points[tmp[j]].y - points[tmp[i]].y < dis; j++) {
            double tmpDis = getDistance(tmp[j], tmp[i]);
            if (cmp(tmpDis, dis) < 0) {
                dis = tmpDis;
                res = { points[tmp[i]], points[tmp[j]] };
            }
        }
    }
    if  (dis < minDis) {
        minDis = dis;
        ans = res;
    }
    return dis;
}

void divideAndConquer() {
    sort(all(points), [&](const pii& A, const pii& B) {
        return A.x != B.x ? A.x < B.x : A.y < B.y;
    });

    divide(0, n - 1);

    cout << "Point " << '[' << ans.first.x << ", " << ans.first.y << "] and point [" 
         << ans.second.x << ", " << ans.second.y 
         << "] has the shortest distance of " << minDis << endl;
}

void solve() {
    input();

    clock_t my_clock = clock(); 
    divideAndConquer();
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