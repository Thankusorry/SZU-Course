#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

typedef struct BiTNode
{
    char data;
    BiTNode *left;
    BiTNode *right;
    int level;
    BiTNode(char a, BiTNode *l, BiTNode *r, int level = 0) : level(level), data(a), left(l), right(r) {}
} BT;

BT *createtree(string s, int &i, int level)
{
    if (s[i] == '#')
        return NULL;
    BT *root = new BT(s[i], nullptr, nullptr);
    root->level = level;
    root->left = createtree(s, ++i, level + 1);
    root->right = createtree(s, ++i, level + 1);

    return root;
}

int getheight(BT *root)
{
    if (!root)
        return 0;
    return max(getheight(root->left), getheight(root->right)) + 1;
}
int max1 = -1, h1 = -1, h2 = -1;
BT *temp;
void MaxDistance(BT *root)
{
    if (!root)
        return;
    int lefth = getheight(root->left);
    int righth = getheight(root->right);
    int distance = lefth + righth;
    if (distance > max1)
    {
        max1 = distance;
        h1 = getheight(root->left);
        h2 = getheight(root->right);
        temp = root;
        // cout << h1 << "_" << h2<<"_"<<temp->level<<endl;
    }
    MaxDistance(root->left);
    MaxDistance(root->right);
}

void inorder(BT *root)
{
    if (!root)
        return;
    inorder(root->left);
    if ((!root->left || !root->right) && max1 != 0)
    {
        if (root->level - temp->level == h1)
        {
            cout << root->data;
            h1 = -1;
        }
        else if (root->level - temp->level == h2)
        {
            cout << " " << root->data;
            h2 = 0;
        }
    }
    inorder(root->right);
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string s;
        cin >> s;
        int i = 0;
        int level = 0;
        BT *root = createtree(s, i, level);
        MaxDistance(root);
        cout << max1 << ":";
        inorder(temp);
        cout << endl;
        max1 = h1 = h2 = -1;
        temp = nullptr;
    }
    return 0;
}