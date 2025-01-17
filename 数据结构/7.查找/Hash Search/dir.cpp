#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

struct node
{
    string name;
    int type;
    vector<node *> child;
    node(string s, int a) : name(s), type(a) {}
};
bool cmp(node *a, node *b)
{
    if (a->type != b->type)
        return a->type > b->type;
    return a->name < b->name;
}
void dfs(node *root, int level)
{
    if (!root)
        return;
    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << root->name<<endl;
    // 先目录后文件
    sort(root->child.begin(), root->child.end(), cmp);
    for (int i = 0; i < root->child.size(); i++)
        dfs(root->child[i], level+1);
}
int main()
{
    int n;
    cin >> n;
    getchar();
    node *root = new node("root", 1), *curr;
    for (int i = 0; i < n; i++)
    {
        string s, temp;
        curr = root;
        getline(cin, s);
        for (int j = 0; j <= s.size(); j++)
        {
            // 1.目录
            if (s[j] == '\\')
            {
                // 查找是否存在
                int flag = 0;
                for (int k = 0; k < curr->child.size(); k++)
                {
                    //
                    if (curr->child[k]->name == temp && curr->child[k]->type == 1)
                    {
                        // 切换到这个目录
                        curr = curr->child[k];
                        flag = 1;
                        break;
                    }
                }
                // 没有这个目录——创建
                if (!flag)
                {
                    node *s = new node(temp, 1);
                    curr->child.push_back(s);
                    curr = s;
                }
                temp.clear();
            }
            else if (j == s.size())
            {
                if (!temp.empty())
                {
                    // 不是\结尾——文件
                    node *s = new node(temp, 0);
                    curr->child.push_back(s);
                }
                temp.clear();
            }
            else
            {
                temp += s[j];
            }
        }
    }
    dfs(root, 0);
}