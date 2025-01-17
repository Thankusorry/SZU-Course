#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

void preorder(vector<int> A, int n, int i)
{
    if (i >= n || A[i] == 0)
        return;
    cout << A[i] << " ";
    preorder(A, n, 2 * i + 1);
    preorder(A, n, 2 * i + 2);
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<int> A(n, 0);
        for (int i = 0; i < n; i++)
            cin >> A[i];
        preorder(A, n, 0);
        cout << endl;
    }
}