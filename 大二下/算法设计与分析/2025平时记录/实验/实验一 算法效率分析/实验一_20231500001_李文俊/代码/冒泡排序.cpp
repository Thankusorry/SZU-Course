// 冒泡排序算法
void BubbleSort(int *data)
{
    // 定义一个整数变量 flag 并初始化为 1，用于标记本轮排序中是否发生了元素交换
    int flag = 1, temp;

    // 外层循环，控制排序的轮数
    // 每次循环结束后，最大的元素会被放到正确的位置，因此可以减少比较的次数
    for (int i = 0; i < n - 1 && flag; i++)
    {
        // 每轮开始时，将 flag 设置为 0，表示假设本轮没有元素交换
        flag = 0;

        // 内层循环，遍历数组中未排序部分的相邻元素
        for (int j = 0; j < n - i - 1; j++)
        {
            // 如果前一个元素大于后一个元素，说明它们的顺序错误，需要交换它们的位置
            if (data[j] > data[j + 1])
            {
                // 发生了交换，将 flag 设置为 1，表示本轮排序还未完成
                flag = 1;
                // 交换 data[j] 和 data[j+1] 的值
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}