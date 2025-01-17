#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

int sum = 0;
typedef struct BiTNode
{
    char data;
    BiTNode *left;
    BiTNode *right;
    BiTNode(char a, BiTNode *l, BiTNode *r) : data(a), left(l), right(r) {}
} BT;

BT *createtree(string s1, string s2)
{
    if (!s1.size())
        return nullptr;
    BT *root = new BT(s1[0], nullptr, nullptr);
    int index = s2.find(s1[0]);
    string leftpre = s1.substr(1, index);
    string leftin = s2.substr(0, index);
    root->left = createtree(leftpre, leftin);
    string rightpre = s1.substr(index + 1);
    string rightin = s2.substr(index + 1);
    root->right = createtree(rightpre, rightin);
    return root;
}

void postorder(BT *root){
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data;
}
int main()
{
    int N;
    cin >> N;
    string s1, s2;
    cin >> s1 >> s2;
    BT *root = createtree(s1, s2);
    postorder(root);
    return 0;
}