// 对数组进行划分 方法一
long Paritition1(long data[], long low, long high)
{
    long pivot = data[high]; // 选择最后一个元素作为枢轴
    long i = low - 1;        // i 是较小元素的索引
    long temp;

    // 遍历数组，从 low 到 high-1
    for (long j = low; j <= high - 1; ++j)
    {
        // 如果当前元素小于或等于枢轴
        if (data[j] <= pivot)
        {
            ++i; // 增加较小元素的索引
            temp = data[i];
            data[i] = data[j];
            data[j] = temp; // 交换 data[i] 和 data[j]
        }
    }
    temp = data[i + 1];
    data[i + 1] = data[high];
    data[high] = temp; // 将枢轴放到正确的位置
    return ++i;        // 返回枢轴的最终位置
}

//对数组进行划分 方法二
long Paritition2(long A[], long low, long high)
{
    long pivot = A[low]; // 选择第一个元素作为枢轴
    while (low < high)
    {
        // 从右向左找第一个小于枢轴的元素
        while (low < high && A[high] >= pivot)
        {
            --high;
        }
        A[low] = A[high]; // 将该元素移到左边
        // 从左向右找第一个大于枢轴的元素
        while (low < high && A[low] <= pivot)
        {
            ++low;
        }
        A[high] = A[low]; // 将该元素移到右边
    }
    A[low] = pivot; // 将枢轴放到正确的位置
    return low;     // 返回枢轴的最终位置
}

// 快速排序
void quickSort(long A[], long low, long high)
{
    if (low < high)
    {
        long pivot = Paritition1(A, low, high); // 获取枢轴的最终位置
        quickSort(A, low, pivot - 1);           // 递归排序左子数组
        quickSort(A, pivot + 1, high);          // 递归排序右子数组
    }
}