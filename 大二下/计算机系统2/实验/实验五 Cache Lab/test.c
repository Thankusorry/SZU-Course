#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <chrono>
#include <random>
#include <iostream>
using namespace std;
#define B 1
#define KB 1024
#define MB 1024 * 1024
#define CACHE_LINE_SIZE 64 // 初始假设值，实际会被覆盖
#define TRIES 1000         // 测试次数
#define THRESHOLD_MULTIPLIER 1.5

random_device rd;
mt19937 gen(rd());
// 测量缓存层次结构和容量
void measure_cache_hierarchy()
{
    const size_t min_size = 1;
    const size_t max_size = 128;
    const size_t step = 1;
    printf("Measuring Cache Hierarchy:\n");
    printf("%-10s %-12s %s\n", "Size(B)", "Time(ns)", "Throughput(MB/s)");

    for (size_t size = min_size; size <= max_size; size *= 2)
    {
        int n = size / sizeof(char);
        uniform_int_distribution<> dis(0, n - 1);
        char *buffer = new char[n];
        int *index = new int[TRIES];
        // 生成随机索引
        for (int i = 0; i < TRIES; i++)
        {
            int temp = dis(gen);
            index[i] = temp;
        }

        double sum = 0;
        for (int i = 0; i < 10; i++)
        {

            auto start = std::chrono::high_resolution_clock::now();
            // 核心测试逻辑：随机访问每个缓存行

            for (int t = 0; t < TRIES; t++)
            {
                buffer[index[t]] = 1;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            double elapsed = duration.count();
            sum += elapsed;
        }
        free(buffer);
        free(index);

        // 计算每次缓存行访问的平均时间
        double avg_time = sum / 10;
        double v = TRIES / (avg_time / 1e9) / (1024 * 1024);
        //        printf("%-10zu %-12.0f %.2f\n", size / KB, avg_time,v);
        printf("%zu,%f,%.2f\n", size, avg_time, v);
    }
}

void measure_cacheline()
{
    const int Size = 20 * MB;
    int n = Size / sizeof(char);
    char *buffer = new char[n];
    volatile char sink; // 防止编译器优化

    cout << "L1 Cache Line:\n";
    printf("%-10s %-12s %s\n", "Size(B)", "Time(ns)", "Throughput(MB/s)");

    for (int stride = 2; stride <= 192; stride *= 2)
    {
        auto start = std::chrono::high_resolution_clock::now();
        int sum = 0;
        for (int offset = 0; offset < stride; offset++)
        { // 外层循环控制偏移
            for (int i = offset; i < n; i += stride)
            { // 内层循环遍历所有偏移位置
                buffer[i] = 1;
                sum++;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double elapsed = duration.count() / 1e6;

        // 计算总数据量和吞吐量
        double throughput = ((sum * sizeof(char)) / elapsed) / (1024 * 1024);

        printf("%-10d %-12.3f %.2f\n", stride, elapsed, throughput);
    }

    delete[] buffer;
}

int main()
{
    // measure_cache_hierarchy();
    measure_cacheline();
    return 0;
}