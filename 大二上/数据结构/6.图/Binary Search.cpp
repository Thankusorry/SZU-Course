#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void BS(vector<int> A, int num)
{
    int n = A.size();
    int l = 0, r = n - 1;
    int mid = (l + r) / 2;
    while (r >= l)
    {
        if (A[mid] == num)
        {
            cout << mid + 1 << endl;
            return;
        }
        else if (A[mid] > num)
        {
            r = mid - 1;
        }
        else
            l = mid + 1;
        mid = (l + r) / 2;
    }
    cout << "error\n";
}
int main()
{
    int n;
    cin >> n;
    vector<int> A(n, 0);
    for (int i = 0; i < n; i++)
        cin >> A[i];
    int t;
    cin >> t;
    while (t--)
    {
        int num;
        cin >> num;
        BS(A, num);
    }
}