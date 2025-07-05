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

double getDistance(pii a, pii b) {
    return hypot(a.x - b.x, a.y - b.y);
}

double divide(int l, int r) {  // 合并: O(n)
    if (l == r) return INF;  // 只包含一个点
    // 因要在两边有序的前提下归并, 故只有两个点时也要排序
    // else if (l + 1 == r) {  // 只包含2个点
    //     double dis = getDistance(points[l], points[r]);
    //     if (dis < minDis) {
    //         minDis = dis;
    //         ans = { points[l], points[r] };
    //     }
    //     return dis;
    // }

    int mid = l + r >> 1;
    int midx = points[mid].x;  // 排序后points[mid]可能改变, 故暂存

    double dis1 = divide(l, mid), dis2 = divide(mid + 1, r);
    double dis = min(dis1, dis2);

    // 归并
    int p = l, q = mid + 1;
    vector<pii> merge;
    while (p <= mid && q <= r) {
        if (points[p].y < points[q].y) merge.push_back(points[p++]);
        else merge.push_back(points[q++]);
    }
    while (p <= mid) merge.push_back(points[p++]);
    while (q <= r) merge.push_back(points[q++]);

    // 将归并结果放回原数组
    int siz = merge.size();
    for (int i = 0; i < siz; i++) points[l + i] = merge[i];

    vector<pii> tmp;  // 在区域[points[mid].x - dis, points[mid].x + dis]中的点
    for (int i = l; i <= r; i++)
        if (abs(points[i].x - midx) <= dis) tmp.push_back(points[i]);
    
    siz = tmp.size();  // siz <= 6
    pair<pii, pii> res;
    for (int i = 0; i < siz; i++) {
        for (int j = i + 1; j < siz && tmp[j].y - tmp[i].y < dis; j++) {
            double tmpDis = getDistance(tmp[j], tmp[i]);
            if (cmp(tmpDis, dis) < 0) {
                dis = tmpDis;
                res = { tmp[i], tmp[j] };
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