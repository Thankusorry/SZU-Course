#include<bits/stdc++.h>
using namespace std;
#define rall(x) (x).rbegin(), (x).rend()
#define endl '\n'

mt19937 rnd(time(0));
int n = 1e9, k = 10;

vector<int> heapSorting() {  // 用堆排序选出序列中最大的10个数
    vector<int> heap(k);
    
    function<void(int, int)> sift = [&](int u, int length)->void {  // 调整以u为根节点的子树
        int ls = u * 2 + 1, rs = u * 2 + 2;  // 节点u的左右儿子节点, 下标从0开始
        if (ls >= length) return;  // 不存在左儿子节点

        if (rs < length) {  // 存在右儿子节点
            if (heap[ls] > heap[rs]) {  // 左儿子节点的值更大
                if (heap[ls] > heap[u]) {  // 左儿子节点的值大于父亲节点的值
                    swap(heap[ls], heap[u]);  // 交换
                    sift(ls, length);  // 调整以ls为根节点的子树
                }
            }
            else {  // 右儿子节点的值更大
                if (heap[rs] > heap[u]) {  // 右儿子节点的值大于父亲节点的值
                    swap(heap[rs], heap[u]);  // 交换
                    sift(rs, length);  // 调整以rs为根的子树
                }
            }
        }
        else {  // 只有左儿子节点
            if (heap[ls] > heap[u]) {  // 左儿子节点的值大于父亲节点的值
                swap(heap[ls], heap[u]);  // 交换
                sift(ls, length);  // 调整以ls为根节点的子树
            }
        }
    };

    function<int(int)> getMin = [&](int length)->int {  // 返回权值最小的节点的下标
        int idx = 0;
        for (int i = 1; i < length; i++) 
            if (heap[i] < heap[idx]) idx = i;
        return idx;
    };

    for (int i = 0; i < k; i++) heap[i] = rnd();  // 取出序列的前10个元素
    for (int i = k / 2; i >= 0; i--) sift(i, k);  // 从最后一个非叶子节点开始调整

    for (int i = k; i < n; i++) {
        int tmp = rnd();
        int idx = getMin(k);
        if (tmp > heap[idx]) {
            heap[idx] = tmp;
            for (int i = k / 2; i >= 0; i--) sift(i, k);  // 调整堆的形态
        }
    }

    vector<int> res;
    for (int i = k - 1; i >= 0; i--) {  // 将堆中的元素存到res中
        res.push_back(heap[0]);  // 记录堆顶元素
        swap(heap[0], heap[i]);  // 将堆顶元素与最后一个叶子节点交换, 即弹出堆顶元素
        sift(0, i);  // 调整堆的形态
    }
    return res;
}

void solve() {
    cin >> n;
    clock_t my_clock = clock(); 
    auto ans1 = heapSorting();
    double timeCost = clock() - my_clock;
    for (int i = 0; i < k; i++) cout << ans1[i] << " \n"[i == k - 1];
    cout << fixed << setprecision(3) << timeCost << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}