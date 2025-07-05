// 动态规划解法：鸡蛋掉落问题（二分优化版）
// 时间复杂度 O(K*N logN)，空间复杂度 O(K*N)
int erfendp(int K, int N)
{
    // 初始化基础情况
    // 当只有0个鸡蛋时无法测试（已默认初始化为0）
    // 当只有1个鸡蛋时，必须逐层测试
    for (int n = 0; n <= N; ++n)
    {
        dp[1][n] = n;
    }

    // 状态转移：从2个鸡蛋开始推导
    for (int e = 2; e <= K; ++e)
    {
        // 处理不同楼层数的情况
        for (int f = 2; f <= N; ++f)
        {
            dp[e][f] = INT_MAX;
            int l = 1, r = f; // 二分搜索的左右边界

            // 使用二分法寻找最优分割点k
            // 目标：找到使 max(broken, notBroken) 最小的k值
            while (l <= r)
            {
                int k = (l + r) / 2;                // 当前测试的楼层
                int broken = dp[e - 1][k - 1];      // 鸡蛋破碎时的子问题
                int notBroken = dp[e][f - k];       // 鸡蛋未碎时的子问题
                int worst = max(broken, notBroken); // 最坏情况

                // 更新最小尝试次数（+1表示本次尝试）
                dp[e][f] = min(dp[e][f], worst + 1);

                // 决策二分方向：
                if (broken < notBroken)
                {
                    l = k + 1; // 破碎次数较少，需要测试更高楼层
                }
                else
                {
                    r = k - 1; // 未碎次数较多，需要测试更低楼层
                }
            }
        }
    }
    return dp[K][N]; // 返回最终解
}
