#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define endl '\n'

int n;

void generateTest1() {  // Test.1: n个相同的数
    string name = ".\\test" + to_string(n) + "\\in1.txt";
    FILE* fp = fopen(name.c_str(), "w");
    for (int i = 0; i < n; i++) fprintf(fp, "%d ", 1);
    fclose(fp);
}

void generateTest2() {  // Test.2: 1 ~ n的升序序列
    string name = ".\\test" + to_string(n) + "\\in2.txt";
    FILE* fp = fopen(name.c_str(), "w");
    for (int i = 0; i < n; i++) fprintf(fp, "%d ", i + 1);
    fclose(fp);
}

void generateTest3() {  // Test.3: n ~ 1的降序序列
    string name = ".\\test" + to_string(n) + "\\in3.txt";
    FILE* fp = fopen(name.c_str(), "w");
    for (int i = n - 1; i >= 0; i--) fprintf(fp, "%d ", i + 1);
    fclose(fp);
}

void generateTest() {  // Test.4 ~ Test.16: 生成随机的1 ~ n的排列
    vector<int> p(n);
    for (int i = 0; i < n; i++) p[i] = i + 1;

    for (int i = 4; i <= 20; i++) {
        string name = ".\\test" + to_string(n) + "\\in" + to_string(i) + ".txt";
        FILE* fp = fopen(name.c_str(), "w");
        
        random_shuffle(all(p));
        for (int i = 0; i < n; i++) fprintf(fp, "%d ", p[i]);
        fclose(fp);
    }
}

void generateTest1e9() {  // 生成一个
    mt19937 rnd(time(0));
    string name = ".\\test1e9\\in.txt";
    FILE* fp = fopen(name.c_str(), "w");  
    for (int i = 0; i < n; i++) fprintf(fp, "%d ", rnd());
    fclose(fp);
}

void solve() {
    cin >> n;

    // generateTest1();
    // generateTest2();
    // generateTest3();
    // generateTest();

    generateTest1e9();
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}