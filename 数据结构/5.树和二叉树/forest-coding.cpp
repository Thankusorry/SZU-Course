#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef struct BiTNode
{
    char data;
    BiTNode *left;
    BiTNode *right;
    BiTNode(char data, BiTNode *l, BiTNode *r) : data(data), left(l), right(r) {}
} BT;

BT *createtree(string A, int &i, int B)
{
    if (A[i] == '0' || i >= A.length())
    {
        i += 2;
        return nullptr;
    }
    BT *root = new BT(A[i], nullptr, nullptr);
    i += 2;
    // 左孩子
    // 可能不在第一个
    int cnt = 0;
    while (!root->left && cnt < B)
    {
        root->left = createtree(A, i, B);
        cnt++;
    }
    // 剩下都是右子树
    BT *temp = root->left;
    for (; cnt < B; cnt++)
    {
        if (A[i] != '0')
        {
            temp->right = createtree(A, i, B);
            temp = temp->right;
        }
        else
            i += 2;
    }

    return root;
}

void coding(BT *root, string code)
{
    if (root->left)
        coding(root->left, code + "0 ");
    if (root->right)
        coding(root->right, code + "1 ");
    if (!root->left && !root->right)
    {
        code.erase(code.length() - 1);
        cout << code << endl;
        // for (int i = 0; i < code.length();i++) {
        //     if(i!=0)
        //         cout << " ";
        //     cout << code[i];
        // }
        // cout << endl;
    }
}
int main()
{
    int N, B;
    cin >> N >> B;
    getchar(); // 读了一个空行
    string A[N];
    BT *root[N];
    for (int i = 0; i < N; i++)
    {
        getline(cin, A[i]); // cin不读空格
        int index = 0;
        root[i] = createtree(A[i], index, B);
    }
    for (int i = 0; i < N - 1; i++)
        root[i]->right = root[i + 1];
    coding(root[0], "");
}