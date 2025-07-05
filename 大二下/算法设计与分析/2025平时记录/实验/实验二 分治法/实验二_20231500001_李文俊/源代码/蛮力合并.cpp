double bruteForce(vector<Point> &points, int left, int right)
{
    double min_distance = numeric_limits<double>::max();
    for (int i = left; i <= right; ++i)
    {
        for (int j = i + 1; j <= right; ++j)
        {
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double distance = dx * dx + dy * dy;
            if (distance < min_distance)
            {
                min_distance = distance;
            }
        }
    }
    return sqrt(min_distance);
}

double closestpair(vector<Point> &points, int left, int right)
{
    int n = right - left + 1;
    if (n <= 3)
        return bruteForce(points, left, right);

    int mid = (left + right) / 2;
    double L = points[mid].x;

    double dl = closestpair(points, left, mid);
    double dr = closestpair(points, mid + 1, right);
    double d = min(dl, dr);
    double min_d = d;

    // 筛选左半和右半的点（|x - L| ≤ d）
    vector<Point> left_points, right_points;
    for (int i = left; i <= right; i++)
    {
        if (points[i].x <= L && L - points[i].x <= d)
        {
            left_points.push_back(points[i]);
        }
        else if (points[i].x > L && points[i].x - L <= d)
        {
            right_points.push_back(points[i]);
        }
    }
    // 蛮力计算中间最小
    for (int i = 0; i < left_points.size(); i++)
    {
        for (int j = 0; j < right_points.size(); j++)
        {
            double dx = right_points[j].x - left_points[i].x;
            double dy = right_points[j].y - left_points[i].y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < d)
            {
                min_d = min(min_d, distance);
            }
        }
    }
    return min_d;
}