#include <bits/stdc++.h>

using namespace std;

// 定义点结构体
struct Point
{
    double x;
    double y;
};

// 创建均匀数据
vector<Point> generateUniformData(int n)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 100000);

    vector<Point> points;
    for (int i = 0; i < n; ++i)
    {
        double x = dis(gen);
        double y = dis(gen);
        points.push_back({x, y});
    }
    return points;
}

// 蛮力法函数
double bruteForce(vector<Point> &points, int left, int right);
// 分治法函数
double closestpair(vector<Point> &points, vector<Point> &pointsy, int left, int right);

int main()
{
    // 大规模数据
    //  for(int n=100000;n<=1000000;n=n+100000){
    // 小规模数据
    for (int n = 10000; n <= 100000; n = n + 10000)
    {
        for (int t = 0; t < 10; t++)
        {
            auto points = generateUniformData(n);
            auto points2 = points, pointy = points;

            // 蛮力法
            cout << "------------------------------------------------\n";
            cout << "蛮力法:";
            cout << "n=" << n << endl;
            double min_distance = numeric_limits<double>::max();
            int cnt = points.size();
            double distance, dx, dy;
            auto start = chrono::high_resolution_clock::now();
            for (int i = 0; i < cnt; ++i)
            {
                for (int j = i + 1; j < cnt; ++j)
                {
                    dx = points[i].x - points[j].x;
                    dy = points[i].y - points[j].y;
                    distance = dx * dx + dy * dy; // 更高效且正确
                    if (distance < min_distance)
                    {
                        min_distance = distance;
                    }
                }
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            cout << "Elapsed time: " << elapsed_seconds.count() * 1000 << " ms" << endl;
            cout << "它们之间的距离是: " << sqrt(min_distance) << endl;

            // 分治法
            cout << "------------------------------------------------\n";
            cout << "分治法：";
            cout << "n=" << n << endl;

            start = chrono::high_resolution_clock::now();
            distance = closestpair(points2, pointy, 0, n - 1);
            elapsed_seconds = end - start;
            cout << "Elapsed time: " << elapsed_seconds.count() * 1000 << " ms" << endl;
            cout << "它们之间的距离是: " << distance << endl;
        }
        // }
    }
    return 0;
}
