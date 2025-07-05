//插入排序算法
void InsertSort(int *data,int n)
{
    // 外层循环，从数组的第二个元素开始，因为第一个元素默认是已排序的
    for (int i = 1; i < n; i++)
    {
        // 将当前元素存储在 temp 中，以便后续插入到正确的位置
        int temp = data[i];
        // 定义一个变量 j，初始化为当前元素的前一个位置
        int j = i - 1;

        // 内层循环，向前遍历已排序的部分，寻找插入 temp 的正确位置
        while (j >= 0 && data[j] > temp)
        {
            // 如果已排序部分的元素大于 temp，则将该元素向后移动一位
            data[j + 1] = data[j];
            // 继续向前遍历
            j--;
        }
        // 找到插入位置后，将 temp 插入到正确的位置
        data[j + 1] = temp;
    }
}