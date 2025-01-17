#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void print(vector<int> &A)
{
    cout << A.size() << " ";
    for (int i = 0; i < A.size(); i++)
    {
        cout << A[i] << " ";
    }
    cout << endl;
}
void insert(vector<int> &A, int index, int value)
{
    int n = A.size();
    if (index < 1 || index > n + 1)
    {
        cout << "ERROR\n";
        return;
    }
    A.insert(A.begin() + index - 1, value);
    print(A);
}
void remove1(vector<int> &A, int index)
{
    if (index < 1 || index > A.size())
    {
        cout << "ERROR\n";
        return;
    }
    A.erase(A.begin() + index - 1);
    print(A);
}
void remove2(vector<int> &A, int value)
{
    vector<int>::iterator iter = find(A.begin(), A.end(), value);
    if (iter == A.end())
    {
        cout << "ERROR\n";
        return;
    }
    A.erase(iter);
    print(A);
}
void search(vector<int> &A, int value)
{
    A.insert(A.begin(), value);
    int cnt = 1;
    for (int i = A.size() - 1; i >= 0; i--, cnt++)
    {
        if (A[i] == A[0] && i != 0)
        {
            cout << "1 " << i << " " << cnt << endl;
            return;
        }
    }
    cout << "0 0 " << A.size() << endl;
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
        print(A);
        int insertindex, insertvalue;
        cin >> insertindex >> insertvalue;
        int index2;
        int value2, value3;
        insert(A, insertindex, insertvalue);
        cin >> index2 >> value2 >> value3;
        remove1(A, index2);
        remove2(A, value2);
        search(A, value3);
    }
}