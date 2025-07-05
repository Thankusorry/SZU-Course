#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define endl '\n'



void solve() {
    // O(n log n)
    // double n1 = 1e5, n0 = 5e5, t0 = 57;
    // cin >> n1;
    // double up = n1 * log2(n1), down = n0 * log2(n0);
    // cout << fixed << setprecision(1) << up * t0 / down << endl;

    // O(n^2)
    // double n1 = 1e5, n0 = 5e5, t0 = 1069330;
    // cin >> n1;
    // double up = n1 * n1, down = n0 * n0;
    // cout << fixed << setprecision(1) << up * t0 / down << endl;

    // O(n log n + k * n)
    // double n1 = 1e5, n0 = 5e5, t0 = 663;
    // cin >> n1;
    // double up = n1 * log2(n1) + 50 * n1;
    // double down = n0 * log2(n0) + 50 * n0;
    // cout << fixed << setprecision(1) << up * t0 / down << endl;

    // O(874 n)
    // double n1 = 1e5, n0 = 5e5, t0 = 872;
    // cin >> n1;
    // double up = 874 * n1 * log2(n1), down = 874 * n0 * log2(n0);
    // cout << fixed << setprecision(1) << up * t0 / down << endl;
    
    // double t1, tt1; cin >> t1 >> tt1;
    // cout << fixed << setprecision(1) << (t1 - tt1) * 100 / t1 << endl;

    for (int i = 0; i < 5; i++) {
        double a; cin >> a;
        cout << log2(a) << endl;
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}