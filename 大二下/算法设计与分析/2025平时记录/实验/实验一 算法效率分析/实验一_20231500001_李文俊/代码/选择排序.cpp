void SelectSort(int *data, int n)
{
    // 外层循环，遍历数组中的每个元素，i表示当前排序的位置
    for (int i = 0; i < n - 1; i++)
    {
        // 假设当前位置i的元素是最小的，用k记录这个最小元素的索引
        int k = i;
        // 内层循环，从当前位置i的下一个元素开始，遍历数组中剩余的元素
        for (int j = i + 1; j < n; j++)
        {
            // 如果找到一个比当前最小元素还小的元素，更新k为这个更小元素的索引
            if (data[k] > data[j])
            {
                k = j;
            }
        }
        // 如果k不等于i，说明找到了比当前位置i更小的元素
        if (k != i)
        {
            // 交换当前位置i的元素和找到的最小元素
            int temp = data[k];
            data[k] = data[i];
            data[i] = temp;
        }
    }
}