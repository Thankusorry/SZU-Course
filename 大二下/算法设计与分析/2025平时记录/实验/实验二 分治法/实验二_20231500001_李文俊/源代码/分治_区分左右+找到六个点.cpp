
/**
 * @brief 分治法寻找最近点对（核心算法）
 * @param points 点集（已按 x 坐标排序）
 * @param left 当前处理区间的左边界
 * @param right 当前处理区间的右边界
 * @return double 最近点对的距离平方
 */
double closestpair(vector<Point> &points, int left, int right)
{
    int n = right - left + 1; // 当前区间点的数量

    // 递归基：如果点数 ≤ 3，直接暴力计算
    if (n <= 3)
    {
        return bruteForce(points, left, right);
    }

    int mid = (left + right) / 2; // 计算中点
    double L = points[mid].x;     // 中线的 x 坐标

    // 递归处理左半部分和右半部分
    double dl = closestpair(points, left, mid);      // 左半部分的最小距离
    double dr = closestpair(points, mid + 1, right); // 右半部分的最小距离
    double d = min(dl, dr);                          // 当前最小距离
    double min_d = d;                                // 初始化最终结果

    // 筛选左半和右半中可能跨越中线的点（|x - L| ≤ d）
    vector<Point> left_points, right_points;
    for (int i = left; i <= right; i++)
    {
        if (points[i].x <= L && L - points[i].x <= d)
        {
            left_points.push_back(points[i]); // 左半候选点
        }
        else if (points[i].x > L && points[i].x - L <= d)
        {
            right_points.push_back(points[i]); // 右半候选点
        }
    }

    // 对右半候选点按 y 坐标排序（便于后续检查）
    sort(right_points.begin(), right_points.end(), cmp2);

    // 检查左半候选点与右半候选点中可能的最近点对
    for (int i = 0; i < left_points.size(); i++)
    {
        // 跳过 y 坐标差距过大的点（优化：减少不必要的比较）
        int j = 0;
        while (j < right_points.size() && right_points[j].y - left_points[i].y >= d)
        {
            j++;
        }
        // 检查右半候选点中最多 6 个可能的点（理论保证）
        for (int k = j; k < j + 6 && k < right_points.size(); k++)
        {
            double dx = right_points[k].x - left_points[i].x;
            double dy = right_points[k].y - left_points[i].y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < d)
            {
                min_d = min(min_d, distance); // 更新最小距离
            }
        }
    }
    return min_d; // 返回最终结果
}
