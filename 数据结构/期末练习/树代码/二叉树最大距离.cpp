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
    BiTNode(char a, BiTNode *l, BiTNode *r, int level) : level(level), data(a), left(l), right(r) {}
} BT;

BT *createtree(string s, int &i, int level)
{
    if (s[i] == '#')
        return nullptr;
    BT *root = new BT(s[i], nullptr, nullptr, level);
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
int max1 = 0, h1 = 0, h2 = 0;
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
        h1 = lefth;
        h2 = righth;
        temp = root;
    }
    MaxDistance(root->left);
    MaxDistance(root->right);
}

void inorder(BT *root)
{
    if (!root)
        return;

    inorder(root->left);
    inorder(root->right);

    if (!root->left && !root->right && max1 != 0)
    {
        if (root->level - temp->level == h1)
        {
            cout << root->data;
            h1 = 0;
        }
        else if (root->level - temp->level == h2)
        {
            cout << " " << root->data;
        }
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
        int i = 0;
        int level = 0;
        BT *root = createtree(s, i, level);
        max1 = h1 = h2 = 0;
        temp = root;
        MaxDistance(root);
        cout << max1 << ":";
        if (!temp->left && temp->right)
        {
            cout << root->data;
            inorder(temp);
        }
        else if (!temp->right && temp->left)
        {
            inorder(temp);
            cout << " " << root->data;
        }
        else
            inorder(temp);

        cout << endl;
    }
    return 0;
}