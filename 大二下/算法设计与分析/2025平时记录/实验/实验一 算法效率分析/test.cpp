#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <cstdlib> // 包含rand()和srand()
#include <ctime>   // 包含time()
using namespace std;

int n = 10000000; // 数组大小                                                               
#define inf 1000000000
// 建小顶堆
void heap_adjust(int arr[], int start, int end) {
    int dad = start;
    int son = dad * 2 + 1;
    while (son <= end) {
        if (son + 1 <= end && arr[son] > arr[son + 1])
            son++; // 选择较小的子节点
        if (arr[dad] <= arr[son])
            return; // 如果父节点小于等于子节点，则不需要调整
        else {
            int temp = arr[dad];
            arr[dad] = arr[son];
            arr[son] = temp; // 交换父节点和子节点的值
            dad = son;
            son = dad * 2 + 1;
        }
    }
}
// 小顶堆解决TOPK问题
void topk(int arr[], int len, int k) {
    int i;
    // 构建大小为k的小根堆
    for (i = k / 2 - 1; i >= 0; i--) {
        heap_adjust(arr, i, k - 1);
    }
    // 遍历剩余元素
    for (i = k; i < len; i++) {
        if (arr[i] > arr[0]) {
            arr[0] = arr[i]; // 替换堆顶元素
            heap_adjust(arr, 0, k - 1); // 调整堆
        }
    }
}


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 三数取中法选择基准值
int medianOfFive(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high]) swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
    return mid;
}

// 三向分区函数
int Paritition(int  data[], int low, int high) {
    int pivot_index = medianOfFive(data,low,high);
    swap(&data[pivot_index],&data[high]);
    int pivot = data[high]; // 枢轴
    int i = low - 1; // i是较小元素的索引
    for (int j = low; j <= high - 1; ++j) {
        // 如果当前元素小于或等于枢轴
        if (data[j] >= pivot) {
            ++i; // 增加较小元素的索引
            temp=data[i];
            data[i]=data[j];
            data[j]=temp;
        }
    }
   swap(&data[high],&data[i+1]); // 将枢轴放到正确的位置
    return i+1; // 返回枢轴的位置
} 

// 快速选择函数
void quickSelect(int A[], int low, int high, int k) {
    while (high > low) {  // 递归转迭代，减少栈开销
        int index = Paritition(A, low, high);
        int numElements = index - low + 1;
        if (numElements == k) return;
        else if (numElements > k) high = index - 1;
        else {
            low = index + 1;
            k -= numElements;
        }
        // 递归处理左半部分
        quickSelect(A, low, high, k);
    }
}

int main() {
    double sum=0,sum1=0;
    int k=10;
    for(int t=0;t<20;t++){
        cout<<"第"<<t+1<<"次排序:";
        int *data = new int[n]; // 动态分配原生数组
        int *data2=new int[n];
        // 随机数生成器设置
        std::random_device rd; // 获取随机数种子
        std::mt19937 gen(rd()); // 初始化Mersenne Twister引擎
        std::uniform_int_distribution<> dis(0, inf); // 定义分布范围，例如0到n*10
        // 填充数组为随机数
        for (int i = 0; i < n; ++i) {
            data[i] = dis(gen); // 生成随机数并存储到数组中
            data2[i]=data[i];
        }

        auto start=chrono::high_resolution_clock::now();
        quickSelect(data,0,n-1,k);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout<<"快速排序耗时: "<<elapsed.count()*1000<<"ms"<<endl;
        sum+=elapsed.count()*1000;
        cout<<endl<<endl;
         cout<<"前十个排序好的数据";
        for(int i=0;i<10;i++){
            cout<<data[i]<<" ";
        }
        cout<<endl<<endl;
        start=chrono::high_resolution_clock::now();
        topk(data2,n,k);
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout<<"堆排序耗时: "<<elapsed.count()*1000<<"ms"<<endl;
        sum1+=elapsed.count()*1000;
        cout<<endl<<endl;
        cout<<"前十个排序好的数据";
        for(int i=0;i<10;i++){
            cout<<data2[i]<<" ";
        }
        cout<<endl<<endl;
        delete []data;
        delete []data2;
    }
    cout<<"随机平均耗时: "<<sum/20<<"ms"<<endl;
    cout<<"堆平均耗时: "<<sum1/20<<"ms"<<endl;
    return 0;
}