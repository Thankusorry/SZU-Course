// 动态规划 + 空间优化的鸡蛋掉落问题解法
// 使用滚动数组减少空间复杂度至O(N)，时间复杂度仍为O(K*N^2)
int pusudp_gd(int K, int N)
{
    // 初始化滚动数组：
    // pre[] 表示上一轮鸡蛋数k-1的状态（类似dp[k-1][...]）
    // cur[] 表示当前鸡蛋数k的状态（类似dp[k][...]）
    int pre[N + 1], cur[N + 1];

    // 初始化边界条件：
    // 当楼层为0时不需要尝试（0次）
    // 当楼层为1时只需要尝试1次（无论多少鸡蛋）
    for (int i = 0; i <= N; i++)
    {
        pre[i] = i; // 当只有1个鸡蛋时，必须线性扫描
    }

    // 主循环：从2个鸡蛋开始逐步计算到K个鸡蛋的情况
    for (int k = 2; k <= K; ++k) // 鸡蛋数量从2递增
    {
        cur[0] = 0; // 0层楼无需尝试

        // 计算当前鸡蛋数k下，从1层到N层的最优解
        for (int n = 1; n <= N; ++n) // 当前楼层数
        {
            cur[n] = INT_MAX; // 初始化当前状态为极大值

            // 尝试在每一层x扔鸡蛋（从1到n层）
            for (int x = 1; x <= n; ++x)
            {
                // 状态转移方程：
                // broken: 鸡蛋破碎时的最坏情况（k-1个鸡蛋，x-1层楼）
                int broken = pre[x - 1];
                // notBroken: 鸡蛋没碎时的最坏情况（k个鸡蛋，n-x层楼）
                int notBroken = cur[n - x];
                // 取两种情况的最大值（最坏情况）加1（本次尝试）
                int worst = 1 + (broken > notBroken ? broken : notBroken);

                // 更新当前状态的最小值
                if (cur[n] > worst)
                {
                    cur[n] = worst;
                }
            }
        }

        // 滚动数组更新：将当前状态pre[]设为上一轮状态cur[]
        for (int i = 0; i < N; i++)
        {
            pre[i] = cur[i];
        }
    }

    // 返回最终结果：K个鸡蛋N层楼的最小尝试次数
    return cur[N];
}
