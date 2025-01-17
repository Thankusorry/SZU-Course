#include <iostream>
#include <string>

using namespace std;

int Getnext(string a, int next[])
{
    int j = 0, k = -1;
    next[0] = -1;
    while (j < a.length())
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

    return next[a.length()];
}
string matched_Prefix_Postfix(string A)
{
    string a;
    int next[A.length() + 1];
    int num = Getnext(A, next);
    if (num)
        a.assign(A, 0, num);
    else
        a = "empty";
    return a;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string A;
        cin >> A;
        string a = matched_Prefix_Postfix(A);
        cout << a << endl;
    }
    return 0;
}