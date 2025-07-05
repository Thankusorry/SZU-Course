// 建小顶堆
void heap_adjust(int arr[], int start, int end)
{
    int dad = start;       // 父节点索引
    int son = dad * 2 + 1; // 左子节点索引
    while (son <= end)
    { // 当子节点在堆内时
        // 选择较小的子节点
        if (son + 1 <= end && arr[son] > arr[son + 1])
            son++;
        // 如果父节点小于等于子节点，则不需要调整
        if (arr[dad] <= arr[son])
            return;
        else
        {
            int temp = arr[dad]; // 交换父节点和子节点的值
            arr[dad] = arr[son];
            arr[son] = temp;
            dad = son;         // 更新父节点索引
            son = dad * 2 + 1; // 更新子节点索引
        }
    }
}

// 小顶堆解决TOPK问题
void topk(int arr[], int len, int k)
{
    int i;
    // 构建大小为k的小根堆
    for (i = k / 2 - 1; i >= 0; i--)
    {
        heap_adjust(arr, i, k - 1);
    }
    // 遍历剩余元素,维护堆
    for (i = k; i < len; i++)
    {
        if (arr[i] > arr[0])
        {                               // 如果当前元素大于堆顶元素
            arr[0] = arr[i];            // 替换堆顶元素
            heap_adjust(arr, 0, k - 1); // 调整堆
        }
    }
}