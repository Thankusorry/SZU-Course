#include <iostream>
#include <vector>
using namespace std;

struct BT
{
    int data;
    BT *lchild;
    BT *rchild;
    BT(int a, BT *p, BT *q) : data(a), lchild(p), rchild(q) {}
    BT() {}
};

int Search(BT *T, int key, BT *parent, BT *&p, int cnt)
{
    if (!T)
    {
        p = parent;
        return -1;
    }
    if (T->data == key)
    {
        return cnt;
    }
    else
    {
        if (T->data > key)
        {
            return Search(T->lchild, key, T, p, cnt + 1);
        }
        else
            return Search(T->rchild, key, T, p, cnt + 1);
    }
}
void inorder(BT *root)
{
    if (!root)
        return;
    inorder(root->lchild);
    cout << root->data << " ";
    inorder(root->rchild);
}
bool Insert(BT *&root, int key)
{
    BT *p;
    if (Search(root, key, nullptr, p, 1) == -1)
    {
        BT *s = new BT(key, nullptr, nullptr);
        if (!p)
        {
            root = s;
        }
        else
        {
            if (p->data > key)
                p->lchild = s;
            else
                p->rchild = s;
        }
        return true;
    }
    return false;
}
BT *createTree(vector<int> A)
{
    BT *root = new BT(A[0], nullptr, nullptr);
    for (int i = 1; i < A.size(); i++)
    {
        Insert(root, A[i]);
    }
    return root;
}
int main()
{
    int n;
    cin >> n;
    vector<int> A(n, 0);
    for (int i = 0; i < n; i++)
        cin >> A[i];
    BT *root = createTree(A);
    inorder(root);
    cout << endl;
    int m;
    cin >> m;
    while (m--)
    {
        int num;
        cin >> num;
        BT *p;
        cout << Search(root, num, nullptr, p, 1);
        cout << endl;
    }
}