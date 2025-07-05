void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 三数取中法选择基准值
int medianOfThree(int arr[], int low, int high)
{
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid])
        swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high])
        swap(&arr[mid], &arr[high]);
    return mid;
}

// 分区函数
int partition(int arr[], int low, int high)
{
    int pivotIndex = medianOfThree(arr, low, high);
    int pivot = arr[pivotIndex];
    swap(&arr[pivotIndex], &arr[high]); // 将基准值移到末尾
    int i = low - 1;                    // 小于基准值的元素的索引
    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// 快速选择函数
int quickSelect(int arr[], int low, int high, int k)
{
    if (low == high)
        return arr[low];

    int pivotIndex = partition(arr, low, high);

    if (k == pivotIndex)
    {
        return arr[k];
    }
    else if (k < pivotIndex)
    {
        return quickSelect(arr, low, pivotIndex - 1, k);
    }
    else
    {
        return quickSelect(arr, pivotIndex + 1, high, k - pivotIndex + low - 1);
    }
}