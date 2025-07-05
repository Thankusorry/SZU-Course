#include <sys/mman.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
const size_t PAGE_SIZE = 4096; // 4KB
const size_t MAX_PAGES = 1024; // 测试最大 4MB 内存

void measure_tlb()
{
    // 分配较大内存块
    size_t buffer_size = MAX_PAGES * PAGE_SIZE;
    char *buffer = (char *)mmap(nullptr, buffer_size, PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
    if (buffer == MAP_FAILED)
    {
        cerr << "mmap failed" << endl;
        return;
    }

    for (int num_pages = 1; num_pages <= MAX_PAGES; num_pages *= 2)
    {
        volatile char sum = 0;
        vector<size_t> page_indices(num_pages);
        iota(page_indices.begin(), page_indices.end(), 0);
        random_shuffle(page_indices.begin(), page_indices.end());

        auto start = chrono::high_resolution_clock::now();
        for (auto idx : page_indices)
        {
            sum += buffer[idx * PAGE_SIZE];
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

        cout << "Pages: " << num_pages
             << "\tTime: " << duration.count() << " ns"
             << "\tAvg per Page: " << duration.count() / num_pages << " ns" << endl;
    }

    munmap(buffer, buffer_size);
}

int main()
{
    measure_tlb();
    return 0;
}