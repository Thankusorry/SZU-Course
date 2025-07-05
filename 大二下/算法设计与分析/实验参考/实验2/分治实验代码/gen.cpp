#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define endl '\n'

const int MAXA = 10;
int n;

void generateTest() {  // 随机生成平面上的n个整点
    mt19937 rnd(time(0));
    string name = ".\\test\\in" + to_string(n) + ".txt";
    FILE* fp = fopen(name.c_str(), "w");  
    for (int i = 0; i < n; i++) {
        int x = rnd(), y = rnd();
        fprintf(fp, "%d %d\n", x % MAXA, y % MAXA);
    }
    fclose(fp);
}

void solve() {
    cin >> n;
    generateTest();
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}