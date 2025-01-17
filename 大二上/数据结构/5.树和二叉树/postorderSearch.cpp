#include <iostream>
using namespace std;

struct BT
{
    char data;
    BT *left, *right;
    BT(char a, BT *b, BT *c) : data(a), left(b), right(c) {}
};
BT *createTree(string str, int &i)
{
    if (str[i] == '0' || i>=str.length())
        return nullptr;
    BT *root = new BT(str[i], nullptr, nullptr);
    root->left = createTree(str, ++i);
    root->right = createTree(str, ++i);
    return root;
}
void postorder(BT *root, string &s)
{
    if (!root)
        return;
    postorder(root->left, s);
    postorder(root->right, s);
    s += root->data;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        string str;
        cin >> str;
        int i = 0;
        BT *root = createTree(str, i);
        string s;
        postorder(root, s);
        for (int i = 0; i < 2; i++)
        {
            char A;
            cin >> A;
            int b;
            if ( s.find(A) == string::npos)
            {
                cout << "ERROR\n";
                continue;
            }
            for (int k = 0; k < s.length(); k++)
            {
                if (s[k] == A)
                {
                    if (k - 1 >= 0)
                    {
                        cout << s[k - 1] << " ";
                    }
                    else
                    {
                        cout << "-1 ";
                    }
                    if (k + 1 < s.length())
                    {
                        cout << s[k + 1] << endl;
                    }
                    else
                    {
                        cout << "-1\n";
                    }
                }
            }
        }
    }
}