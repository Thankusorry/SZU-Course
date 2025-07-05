#include<iostream>
#include<chrono>
#include<random>
#define KB 1024
#define MB (1024*1024)
using namespace std;
random_device rd;//随机数生成
mt19937 mt(rd());
void random_access(int size) {
	char *buffer = new char[size];
	uniform_int_distribution<> distribution(0, size - 1);
	int times = 10 * 1000 * 1000;
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < times; i++) {
		buffer[distribution(mt)] = 0;
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << (size / 1024) << "KB " << duration.count() << "ms" << endl;
	delete[] buffer;
}
void measure_cache_rank() {
	vector<int> sizes{8 * KB, 16 * KB, 32 * KB, 64 * KB, 128 * KB, 256 * KB, 512 * KB, 1 * MB, 2 * MB, 4 * MB, 6 * MB,
	                  8 * MB, 10 * MB, 12 * MB, 14 * MB, 16 * MB, 18 * MB, 20 * MB, 22 * MB, 24 * MB};
	for (auto size : sizes) {
		random_access(size);
	}
}
void stride_access(int stride) {
	int size = 256 * KB;
	char *buffer = new char[size];
	auto start = chrono::high_resolution_clock::now();
	int times = 1000;
	while (times--) {
		for (int j = 0; j < stride; j++) {
			for (int i = 0; i < size; i += stride) {
				buffer[i] = 0;
			}
		}
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << stride << "B " << duration.count() << "ms" << endl;
	delete[] buffer;
}
void measure_cache_L1() {
	vector<int> strides{1, 2, 4, 8, 16, 32, 48, 64, 96, 128, 192, 256};
	for (auto stride : strides) {
		stride_access(stride);
	}
}
int main() {
	measure_cache_rank();
    cout << endl;
	measure_cache_L1();
	return 0;
}
