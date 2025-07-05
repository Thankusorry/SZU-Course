#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define endl '\n'

int n;
vector<int> a;  // 欲排序的序列
vector<int> tmp;  // 归并排序中用于临时存放有序序列的数组

void selectSorting() {  // 朴素选择排序
    for (int i = 0; i < n - 1; i++) {  // 进行(n - 1)轮迭代
        int minidx = i;
        for (int j = i + 1; j < n; j++)  // 非降序排列, 找到后缀中的最小元素
            if (a[j] < a[minidx]) minidx = j;
        if (minidx != i) swap(a[i], a[minidx]);  // 交换a[i]与最小元素
    }
}

void bubbleSorting() {  // 朴素冒泡排序
    for (int i = 0; i < n - 1; i++) {  // 进行(n - 1)轮迭代
        for (int j = 0; j < i; j++)  // 检查相邻两元素的大小关系
            if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
    }
}

void mergeSorting() {  // 递归实现的2路-归并排序
    tmp.resize(n + 1);
    
    function<void(int, int)> mergeSorting = [&](int l, int r)->void {
        if (l >= r) return;  // 递归终止条件

        int mid = l + r >> 1;
        mergeSorting(l, mid), mergeSorting(mid + 1, r);  // 递归排序左右两边

        // 合并两有序序列
        int idx = 0;  // tmp[]数组当前用到的下表
        int i = l, j = mid + 1;  // 指针i、j分别指向左右两序列的起点
        while (i <= mid && j <= r) {
            if (a[i] <= a[j]) tmp[idx++] = a[i++];
            else tmp[idx++] = a[j++];
        }

        while (i <= mid) tmp[idx++] = a[i++];  // 左边序列没放完
        while (j <= r) tmp[idx++] = a[j++];  // 右边序列没放完

        for (int i = l, j = 0; i <= r; i++, j++) a[i] = tmp[j];  // 将排序结果放回原数组
    }; 
    mergeSorting(1, n);
}

void quickSorting() {  // 递归实现的快速排序
    function<void(int, int)> quickSorting = [&] (int l, int r)->void {
        if (l >= r) return;

        // 将元素按与pivot的大小关系划分
        int i = l - 1, j = r + 1;  // 指针i、j分别指向欲排序区间的[l, r]的前一位、后一位
        int pivot = a[l + r >> 1];  // 比较的标准
        while (i < j) {
            do i++; while (a[i] < pivot);  // 指针i指向第一个 >= pivot的元素
            do j--; while (a[j] > pivot);  // 指针j指向第一个 <= pivot的元素
            if (i < j) swap(a[i], a[j]);
        }

        quickSorting(l, j), quickSorting(j + 1, r);  // 递归排序两边
    };
    quickSorting(1, n);
}

void insertSorting() {  // 朴素插入排序
    for (int i = 1; i < n; i++) {  // 当前a[0 ... i]有序
        for (int j = i - 1; j >= 0 && a[j] > a[j + 1]; j--)
            swap(a[j], a[j + 1]);
    }
}

void solve() {
    cin >> n;
    a.resize(n + 1);

    double totalTime = 0;
    for (int i = 1; i <= 20; i++) {
        string name = ".\\test" + to_string(n) + "\\in" + to_string(i) + ".txt";
        freopen(name.c_str(), "r", stdin);
        
        // 选择排序、冒泡排序、插入排序: 序列下标从0开始
        for (int j = 0; j < n; j++) cin >> a[j];
        clock_t my_clock = clock(); 
        selectSorting();
        // bubbleSorting();
        // insertSorting();

        // 归并排序、快速排序: 序列下标从1开始
        // for (int j = 1; j <= n; j++) cin >> a[j];
        // clock_t my_clock = clock(); 
        // mergeSorting();
        // quickSorting();

        double timeCost = clock() - my_clock;
        totalTime += timeCost;
    }
    cout << fixed << setprecision(3) << totalTime / 20 << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}