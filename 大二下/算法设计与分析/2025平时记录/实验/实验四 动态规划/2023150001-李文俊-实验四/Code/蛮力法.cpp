// 函数bruteForce：使用蛮力递归方法解决鸡蛋掉落问题
// 参数：
//   e - 鸡蛋的数量
//   f - 楼层的总数
// 返回值：
//   返回在最坏情况下，找到临界楼层所需的最少尝试次数
int bruteForce(int e, int f)
{
    // 边界情况处理：
    // 如果只有0层或1层楼，那么尝试次数就是楼层数本身
    if (f == 0 || f == 1)
        return f;
    // 如果只有1个鸡蛋，必须从下往上逐层尝试，所以尝试次数等于楼层数
    if (e == 1)
        return f;

    // 初始化最小尝试次数为最大整数值
    int minTry = INT_MAX;

    // 从第1层到第f层，依次尝试每一层楼
    for (int k = 1; k <= f; ++k)
    {
        // 计算如果在第k层扔下鸡蛋，鸡蛋碎了的情况
        // 此时需要在下面的k-1层楼中继续测试，使用e-1个鸡蛋
        int broken = bruteForce(e - 1, k - 1);

        // 计算如果在第k层扔下鸡蛋，鸡蛋没碎的情况
        // 此时需要在上面的f-k层楼中继续测试，使用e个鸡蛋
        int notBroken = bruteForce(e, f - k);

        // 当前情况的最坏情况尝试次数是以上两种情况的较大值，再加上当前这一次尝试
        int worst = 1 + max(broken, notBroken);

        // 更新最小尝试次数
        minTry = min(minTry, worst);
    }

    // 返回所有可能尝试中的最小尝试次数
    return minTry;
}
