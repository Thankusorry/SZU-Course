/**
 * @brief 分治法寻找最近点对（优化版）
 * @param points 按x坐标排序的点集
 * @param pointsy 按y坐标排序的点集
 * @param left 当前处理区间的左边界
 * @param right 当前处理区间的右边界
 * @return double 最近点对的距离平方
 */
double closestpair(vector<Point> &points, vector<Point> &pointsy, int left, int right)
{
    int n = right - left + 1; // 当前区间点的数量

    // 递归基：点数 ≤ 3时直接暴力计算
    if (n <= 3)
    {
        return bruteForce(points, left, right);
    }

    int mid = (left + right) / 2; // 计算中点索引
    double L = points[mid].x;     // 中线的x坐标

    // 划分已排序的y坐标数组（Ly: 左半部分y排序，Ry: 右半部分y排序）
    vector<Point> Ly, Ry;
    for (int i = 0; i < n; ++i)
    {
        if (pointsy[i].x <= L)
        {
            Ly.push_back(pointsy[i]); // 左半候选点（x ≤ L）
        }
        else
        {
            Ry.push_back(pointsy[i]); // 右半候选点（x > L）
        }
    }

    // 递归处理左半部分和右半部分
    double dl = closestpair(points, Ly, left, mid);      // 左半最小距离
    double dr = closestpair(points, Ry, mid + 1, right); // 右半最小距离
    double d = min(dl, dr);                              // 当前最小距离
    double min_d = d;                                    // 初始化最终结果

    // 收集距离中线L不超过d的点（形成strip区域）
    vector<Point> strip;
    for (int i = 0; i <= pointsy.size(); i++)
    {
        if (abs(pointsy[i].x - L) < d)
        {
            strip.push_back(pointsy[i]); // 加入strip区域
        }
    }

    // 检查strip区域内的点对（最多检查后续7个点）
    double dx, dy, dist;
    for (int i = 0; i < strip.size(); ++i)
    {
        // 由于strip已按y排序，只需检查附近点（几何性质保证最多7个）
        for (int j = i + 1; j < strip.size() && j < i + 8; ++j)
        {
            dx = strip[j].x - strip[i].x;
            dy = strip[j].y - strip[i].y;
            dist = sqrt(dx * dx + dy * dy); // 距离平方
            if (dist < min_d)
            {
                min_d = dist; // 更新最小距离
            }
        }
    }
    return min_d; // 返回最终结果
}

int main()
{
    // 测试不同规模的点集（从10万到100万，步长10万）
    for (int n = 100000; n <= 1000000; n += 100000)
    {
        // 生成均匀分布的点集
        auto points = generateUniformData(n);
        auto points2 = points; // 备份用于x排序
        auto pointy = points;  // 备份用于y排序

        // 分治法处理
        cout << "------------------------------------------------\n";
        cout << "分治法：n = " << n << endl;

        // 预处理：分别按x和y排序
        sort(points2.begin(), points2.end(), cmp); // 按x排序
        sort(pointy.begin(), pointy.end(), cmp2);  // 按y排序

        // 计时开始
        auto start = chrono::high_resolution_clock::now();
        double distance = closestpair(points2, pointy, 0, n - 1);
        auto end = chrono::high_resolution_clock::now();

        // 输出结果
        chrono::duration<double> elapsed = end - start;
        cout << "Elapsed time: " << elapsed.count() * 1000 << " ms\n";
        cout << "最近距离: " << distance << endl;
    }
    return 0;
}
