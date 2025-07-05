int n;
void SelectSort(int *data)
{
    for (int i = 0; i < 10; i++) // 进行10次，选出10个最大的
    {
        int k = i;
        for (int j = i + 1; j < n; j++)
        {
            if (data[k] > data[j])
            {
                k = j;
            }
        }
        if (k != i)
        {
            int temp = data[k];
            data[k] = data[i];
            data[i] = temp;
        }
    }
}