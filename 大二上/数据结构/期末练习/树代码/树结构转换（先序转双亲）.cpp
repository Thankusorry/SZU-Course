#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <queue>
using namespace std;

typedef struct BiTNode
{
    char data;
    BiTNode *lchild;
    BiTNode *rchild;
    int parentindex;
    BiTNode(char d, BiTNode *l, BiTNode *r) : data(d), lchild(l), rchild(r) {}
} BT;
struct T
{
    char data;
    int parent;
    T(char a, int p) : data(a), parent(p) {}
};
BT *createtree(string A, int &i)
{
    if (A[i] == '#')
        return nullptr;
    BT *root = new BT(A[i], nullptr, nullptr);
    root->lchild = createtree(A, ++i);
    root->rchild = createtree(A, ++i);
    return root;
}
void levelorder(BT *root)
{
    vector<T> A;
    if (!root)
        return;
    BT *curr = root;
    queue<BT *> q;
    root->parentindex = -1;
    int index = 0;
    q.push(root);
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        A.push_back(T(curr->data, curr->parentindex));

        if (curr->lchild)
        {
            q.push(curr->lchild);
            curr->lchild->parentindex = index;
        }
        if (curr->rchild)
        {
            q.push(curr->rchild);
            curr->rchild->parentindex = index;
        }
        index++;
    }
    for (int i = 0; i < A.size(); i++)
    {
        if (i)
            cout << " ";
        cout << A[i].data;
    }
    cout << endl;
    for (int i = 0; i < A.size(); i++)
    {
        if (i)
            cout << " ";
        cout << A[i].parent;
    }
    cout << endl;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string bitree;
        cin >> bitree;
        int i = 0;
        BT *root = createtree(bitree, i);
        levelorder(root);
    }
}