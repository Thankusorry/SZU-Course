#include <iostream>
#include <string>

using namespace std;

void Getnext(string a, int next[])
{
    int j = 0, k = -1;
    next[0] = -1;
    while (j < a.length() - 1)
    {
        if (k == -1 || a[j] == a[k])
        {
            // next[j + 1] = k + 1;
            // j++;
            // k++;
            next[++j] = ++k;
        }
        else
        {
            k = next[k];
        }
    }
    // for (int i = 0; i < a.length(); i++)
    // {
    //     cout << next[i] << " ";
    // }
    // cout << endl;
}
int KMP(string A, string a, int next[])
{
    int i = 0, j = 0;
    for (; i < A.length() && j < a.length();)
    {
        if (j == -1 || A[i] == a[j])
        { //
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    if (j == a.length())
        return i - j + 1;
    return 0;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string A, a, b;
        cin >> A >> a >> b;
        cout << A << endl;
        int next[a.length()];
        Getnext(a, next);
        int flag = KMP(A, a, next);
        if (flag){
            A.replace(flag - 1, a.length(),b);
            // A.erase(flag - 1, a.length());
            // A.insert(flag - 1,b);
        }

        cout << A << endl;
    }
    return 0;
}