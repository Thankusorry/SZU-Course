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
void levelorder(BT *root)
{
    if (!root)
        return;
    BT *curr;
    queue<BT *> q;
    q.push(root);
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        cout << curr->data;
        if (curr->left)
            q.push(curr->left);
        if (curr->right)
            q.push(curr->right);
    }
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
        levelorder(root);
        cout << endl;
    }
}