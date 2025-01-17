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

bool Search(BT *T, int key, BT *parent, BT *&p)
{
    if (!T)
    {
        p = parent;
        return false;
    }
    if (T->data == key)
    {
        return true;
    }
    else
    {
        p = T;
        if (T->data > key)
            return Search(T->lchild, key, T, p);

        else
            return Search(T->rchild, key, T, p);
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
bool Insert(BT *root, int key)
{
    BT *p;
    if (!Search(root, key, nullptr, p))
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
bool Delete(BT *&root, BT *parent)
{
    BT *p = root;
    if (!root->lchild)
    {
        if (parent->rchild == p)
        {
            parent->rchild = root->rchild;
        }
        else
            parent->lchild = root->rchild;
        delete p;
    }
    else if (!root->rchild)
    {
        if (parent->rchild == p)
            parent->rchild = root->lchild;
        else
            parent->lchild = root->lchild;
        delete p;
    }
    else
    {
        BT *q = root->lchild;
        while (q->rchild)
        {
            p = q;
            q = q->rchild;
        }
        root->data = q->data;
        if (!root->lchild->rchild)
        {
            root->lchild = q->lchild;
            delete q;
        }
        else
        {
            p->rchild = q->lchild;
            delete q;
        }
    }

    return true;
}
int DeleteBST(BT *&root, int key)
{
    BT *p = nullptr, *q = root;
    while (q)
    {
        if (q->data == key)
            break;
        p = q;
        if (q->data < key)
            q = q->rchild;
        else
            q = q->lchild;
    }
    if (!q)
        return false;
    else if (!p)
    {
        return Delete(root, root);
    }
    else
        return Delete(q, p);
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
        BT *root = createTree(A);
        inorder(root);
        cout << endl;
        int m;
        cin >> m;
        while (m--)
        {
            int num;
            cin >> num;
            DeleteBST(root, num);
            inorder(root);
            cout << endl;
        }
    }
}