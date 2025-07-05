/**
 * @brief 使用暴力法（Brute Force）在给定的点集中寻找最近点对的距离（平方）
 *
 * @param points 点集，每个点包含 x 和 y 坐标
 * @param left 当前处理区间的左边界（包含）
 * @param right 当前处理区间的右边界（包含）
 * @return double 返回最近点对的 距离平方（未开平方根，避免浮点运算开销）
 */
double bruteForce(vector<Point> &points, int left, int right)
{
    // 初始化最小距离为 double 类型的最大值
    double min_distance = numeric_limits<double>::max();

    // 外层循环：遍历所有点对中的第一个点（i）
    for (int i = left; i <= right; ++i)
    {
        // 内层循环：遍历所有点对中的第二个点（j = i+1 避免重复计算）
        for (int j = i + 1; j <= right; ++j)
        {
            // 计算两点在 x 轴和 y 轴上的差值
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;

            // 计算两点距离的平方（未开平方根，节省计算开销）
            double distance = dx * dx + dy * dy;

            // 如果当前距离比记录的最小距离更小，则更新最小距离
            if (distance < min_distance)
            {
                min_distance = distance;
            }
        }
    }

    // 返回最近点对的距离平方（调用方可根据需要开平方根）
    return sqrt(min_distance);
}
