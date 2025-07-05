const int MAX_K = 10000; // 最大鸡蛋数
const int MAX_N = 10000; // 最大楼层数

int dp[MAX_K][MAX_N]; // dp[i][j] 表示 i 个鸡蛋，j 层楼的最坏情况
int pusudp(int K, int N)
{
    // 动态规划填充 dp 数组
    // 外层循环：遍历鸡蛋数量，从 2 开始到 K
    for (int k = 2; k <= K; ++k)
    {
        // 中层循环：遍历楼层数，从 2 开始到 N
        for (int n = 2; n <= N; ++n)
        {
            // 初始化当前状态为最大整数值，便于后续比较
            dp[k][n] = INT_MAX;

            // 内层循环：尝试在每一层楼扔鸡蛋，从第 1 层到第 n 层
            for (int x = 1; x <= n; ++x)
            {
                // 计算鸡蛋破碎的情况下的最坏情况尝试次数
                int broken = dp[k - 1][x - 1]; // 如果鸡蛋碎了，则剩下 k-1 个鸡蛋和 x-1 层楼

                // 计算鸡蛋没有破碎的情况下的最坏情况尝试次数
                int notBroken = dp[k][n - x]; // 如果鸡蛋没碎，则剩下 k 个鸡蛋和 n-x 层楼

                // 获取当前尝试的最坏情况，即两种情况中的最大值再加一次尝试
                int worst = 1 + (broken > notBroken ? broken : notBroken);

                // 更新 dp[k][n] 为所有可能 x 中最小的最坏情况
                if (worst < dp[k][n])
                {
                    dp[k][n] = worst;
                }
            }
        }
    }

    // 返回最终结果：使用 K 个鸡蛋和 N 层楼时的最小尝试次数
    return dp[K][N];
}
