#include <iostream>
#include <queue>
#include <cmath>
using namespace std;
struct tree
{
    char data;
    int parent;
};

void preorder(vector<tree> A, int n, int i)
{
    cout << A[i].data;
    for (int j = 0; j < n; j++)
    {
        if (A[j].parent == i)
        {
            preorder(A, n, j);
        }
    }
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<tree> A(n);
        for (int i = 0; i < n; i++)
            cin >> A[i].data;
        for (int i = 0; i < n; i++)
            cin >> A[i].parent;
        int i;
        for (i = 0; i < n; i++)
        {
            if (A[i].parent == -1)
                break;
        }
        preorder(A, n, i);
        cout << endl;
    }
    return 0;
}