#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

struct BT
{
    char data;
    BT *left, *right;
    BT(char a, BT *b, BT *c) : data(a), left(b), right(c) {}
};
BT *createTree(string A, int n, int &i)
{
    if (A[i] == '0' || i >= n)
        return nullptr;
    BT *root = new BT(A[i], nullptr, nullptr);
    root->left = createTree(A, n, ++i);
    root->right = createTree(A, n, ++i);
    return root;
}
int getheight(BT *root) {
    if(!root)
        return 0;
    return max(getheight(root->left), getheight(root->right)) + 1;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string s;
        cin >> s;
        int n = s.length();
        int i = 0;
        BT *root = createTree(s, n, i);
        int h=getheight(root);
        cout << h<<endl;
    }
}