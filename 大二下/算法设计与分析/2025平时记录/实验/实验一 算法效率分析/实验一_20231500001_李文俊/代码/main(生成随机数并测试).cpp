#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <cstdlib>   // 包含rand()和srand()
#include <ctime>     // 包含time()
using namespace std;

/*
排序函数
*/


int main()
{
    double sum = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0; // 记录运行时间之和
    int k = 10;                                                                 // TopK问题
    int n = 100000;                                                             // 数据规模
    for (int t = 0; t < 20; t++)                                                // 样本为20
    {
        cout << "第" << t + 1 << "次排序:";
        // 动态分配原生数组
        int *data = new int[n];
        int *data2 = new int[n];
        int *data3 = new int[n];
        int *data4 = new int[n];
        int *data5 = new int[n];
        int *data6 = new int[n];
        // 随机数生成器设置
        std::random_device rd;                          // 获取随机数种子
        std::mt19937 gen(rd());                         // 初始化Mersenne Twister引擎
        std::uniform_int_distribution<> dis(0, n * 10); // 定义分布范围，例如0到n*10
        // 填充数组为随机数
        for (int i = 0; i < n; ++i)
        {
            data[i] = dis(gen); // 生成随机数并存储到数组中
            data2[i] = data[i];
            data3[i] = data[i];
            data4[i] = data[i];
            data5[i] = data[i];
            data6[i] = data[i];
        }
//第一个函数用时        
        // 高精度时间计算
        auto start = chrono::high_resolution_clock::now();
        // 放入排序函数
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "xx排序耗时: " << elapsed.count() * 1000 << "ms" << endl;
        sum += elapsed.count() * 1000;

// 第二个函数用时
        start = chrono::high_resolution_clock::now();
        // 排序函数
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "xx排序耗时: " << elapsed.count() * 1000 << "ms" << endl;
        sum1 += elapsed.count() * 1000;

// 第三个函数用时
        start = chrono::high_resolution_clock::now();
        // 排序函数
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "xx排序耗时: " << elapsed.count() * 1000 << "ms" << endl;
        sum2 += elapsed.count() * 1000;

//第四个函数用时 
        start = chrono::high_resolution_clock::now();
        // 排序函数
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "xx排序耗时: " << elapsed.count() * 1000 << "ms" << endl;
        sum3 += elapsed.count() * 1000;

//第五个函数用时 
        start = chrono::high_resolution_clock::now();
        // 排序函数
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        cout << "xx排序耗时: " << elapsed.count() * 1000 << "ms" << endl;
        sum4 += elapsed.count() * 1000;

        delete[] data;
        delete[] data2;
        delete[] data3;
        delete[] data4;
        delete[] data5;
        delete[] data6;
    }
    
//平均用时
    cout << "xx平均耗时: " << sum / 20*1000 << "ms" << endl;
    cout << "xx平均耗时: " << sum1 / 20*1000 << "ms" << endl;
    //后面有需要可增加
    return 0;
}