#include <iostream>
#include <queue>
#include <cmath>
using namespace std;
struct tree
{
    char data;
    vector<int> kids;
};

void postorder(vector<tree> A, int n, int i)
{
    for (int j = 0; j < A[i].kids.size(); j++)
    {
        postorder(A, n, A[i].kids[j]);
    }
    cout << A[i].data;
}

int main()
{
    int n;
    cin >> n;
    int Iroot;
    cin >> Iroot;
    vector<tree> A(n);
    for (int i = 0; i < n; i++)
    {
        cin >> A[i].data;
        int num;
        cin >> num;
        while (num != -1)
        {
            A[i].kids.push_back(num);
            cin >> num;
        }
    }
    int i;
    postorder(A, n, Iroot);
    cout << endl;

    return 0;
}