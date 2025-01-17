#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

typedef struct BiTNode
{
    char data;
    BiTNode *left;
    BiTNode *right;
    int level;
    BiTNode(char data, BiTNode *l, BiTNode *r) : data(data), left(l), right(r) {}
} BT;

// BT *createtree(string A, int &i)
// {
//     if(A[i]=='-' || i>=A.length())
//         return nullptr;
//     BT *root=new BT(A[i],nullptr,nullptr);
//     root->left = createtree(A, ++i);
//     root->right = createtree(A, ++i);
//     return root;
// }
/* 最后一题只关心双亲（孩子）。且不关心双亲结点下标，双亲存结点本身就可以。存下来后，双亲双亲滚动走就OK。代码很短。不要想复杂了。 */
// index2不动
int search(int index1, int index2, vector<int> fa, vector<int> ma,int depth=0)
{
    int father = fa[index1], mother = ma[index1];
    if(father ==index2 || mother==index2)
        return depth + 1;
    int F=-1, M=-1;
    if(father != -1)
     F= search(father, index2, fa, ma,depth+1);
    if (mother != -1)
     M = search(mother, index2, fa, ma,depth+1);
    if (father == -1 && mother == -1)
        return -1;
    if(F!=-1)
        return F;
    if(M!=-1)
        return M;
    return -1;
}
int main()
{
    int n, m;
    cin >> n >> m;
    while (1)
    {
        if (n == 0 && m == 0)
            break;
        vector<int> fa(100, -1), ma(100, -1);
        for (int i = 0; i < n; i++)
        {
            char a, b, c;
            cin >> a >> b >> c;
            if (b != '-')
                fa[a - 'A' + 1] = b - 'A' + 1;
            if (c != '-')
                ma[a - 'A' + 1] = c - 'A' + 1;
        }
        for (int i = 0; i < m; i++)
        {
            char a, b;
            cin >> a >> b;
            int flag=search(b - 'A' + 1, a - 'A' + 1, fa, ma);// a是b祖先
            if(flag==1) cout<<"parent\n";
            else if (flag==2)
                cout << "grandparent\n";
            else if(flag>2) {
                for (int j = 0; j < flag -2;j++)
                    cout << "great-";
                    cout << "grandparent\n";
            }
            else{
                flag = search(a - 'A' + 1, b - 'A' + 1, fa, ma);
                if (flag == 1)
                    cout << "child\n";
                else if (flag == 2)
                    cout << "grandchild\n";
                else if (flag > 2)
                {
                    for (int j = 0; j < flag - 2; j++)
                        cout << "great-";
                    cout << "great-grandchild\n";
                }
                else {
                    cout << "-\n";
                }
            }
        }
        cin >> n >> m;
    }
    // cout << (int)'-' << endl; 45
}