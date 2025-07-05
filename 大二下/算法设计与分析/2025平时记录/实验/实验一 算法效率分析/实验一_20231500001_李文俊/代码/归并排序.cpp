// 合并过程
void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1; // 左子数组的大小
    int n2 = right - mid;    // 右子数组的大小

    // 创建临时数组 L 和 R
    int L[n1], R[n2];

    // 将数据拷贝到临时数组 L 和 R
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // 合并临时数组回原数组
    int i = 0;    // 初始化左子数组索引
    int j = 0;    // 初始化右子数组索引
    int k = left; // 初始化合并后的数组索引

    // 合并两个有序子数组
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }

    // 拷贝剩余元素
    while (i < n1)
    {
        arr[k++] = L[i++];
    }

    while (j < n2)
    {
        arr[k++] = R[j++];
    }
}

// 对数组进行归并排序
void mergeSort(int data[], int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        mergeSort(data, left, mid);      // 递归排序左半部分
        mergeSort(data, mid + 1, right); // 递归排序右半部分
        merge(data, left, mid, right);   // 合并两个有序子数组
    }
}