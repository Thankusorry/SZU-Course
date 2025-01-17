### D. 红色警报时间限制

#### 题目描述

战争中保持各个城市间的连通性非常重要。本题要求你编写一个报警程序，当失去一个城市导致国家被分裂为多个无法连通的区域时，就发出红色警报。注意：若该国本来就不完全连通，是分裂的k个区域，而失去一个城市并不改变其他城市之间的连通性，则不要发出警报。

#### 输入

输入在第一行给出两个整数`N`（0 < `N` ≤ 500）和`M`（≤ 5000），分别为城市个数（于是默认城市从0到`N`-1编号）和连接两城市的通路条数。随后`M`行，每行给出一条通路所连接的两个城市的编号，其间以1个空格分隔。在城市信息之后给出被攻占的信息，即一个正整数`K`和随后的`K`个被攻占的城市的编号。

注意：输入保证给出的被攻占的城市编号都是合法的且无重复，但并不保证给出的通路没有重复。

```
5 4
0 1
1 3
3 0
0 4
5
1 2 0 4 3
```

#### 输出

对每个被攻占的城市，如果它会改变整个国家的连通性，则输出`Red Alert: City k is lost!`，其中`k`是该城市的编号；否则只输出`City k is lost.`即可。如果该国失去了最后一个城市，则增加一行输出`Game Over.`。

```
City 1 is lost.
City 2 is lost.
Red Alert: City 0 is lost!
City 4 is lost.
City 3 is lost.
Game Over.
```

+++

刚开始想偷懒用那个Floyd算法，看起来时间复杂度就爆炸，然后统计断开后每次不连通的个数，差距>1 就报错，但好多细节不知道怎么处理

比如，开始就不连通，还是有一个致命缺陷：就是断开节点后其他不能相连的没有断开，**无法更新状态！**

怎么处理?

### 处理联通：并查集：

**并查集：**一种树形的数据结构，用于处理不相交的集合合并(union)及查询(find)问题。

**初始**，图中各顶点为一棵子树的根，即`parent(i)= -1；i=0,…,n-1(顶点数n)`

设加入**（i,j) **边,通过parent**向上递推查找i顶点的子树根**，设为k，同理 j 顶点的子树根，设为m**(find)**。若k≠m,令parent(k)=m;加入(i,j)边`（union)`。否则不加入(i,j在同一棵子树）

**FIND:确定元素属于哪一个子集**

**UNION：将两个子集合并为同一个集合。**

**QUICK FIND,QUICK UNION**

<img src="https://liwenjunpictures.oss-cn-shenzhen.aliyuncs.com/202411082303882.png" alt="image-20241108230330778" style="zoom:50%;" />

1.以下标为根的并查集

程序实现：

1.find——找教主

```cpp
int find(int x)					//查找x的教主
{
    while(pre[x] != x)			//如果x的上级不是自己（则说明找到的人不是教主）
        x = pre[x];				//x继续找他的上级，直到找到教主为止
    return x;					//教主驾到~~~
}
```

简化版：

```cpp
int find(int x)     				//查找结点 x的根结点 
{
    if(pre[x] == x) return x;		//递归出口：x的上级为 x本身，即 x为根结点        
    return pre[x] = find(pre[x]);	//此代码相当于先找到根结点 rootx，然后pre[x]=rootx 
}
将x到根节点路径上的所有点的pre（上级）都设为根节点
```

该算法存在一个缺陷：只有当查找了某个节点的代表元（教主）后，才能对该查找路径上的各节点进行路径压缩。换言之，第一次执行查找操作的时候是实现没有压缩效果的，只有在之后才有效。

2.Union——收徒！

```cpp
void join(int x,int y)                     //我想让虚竹和周芷若做朋友
{
    int fx=find(x), fy=find(y);            //虚竹的老大是玄慈，芷若MM的老大是灭绝
    if(fx != fy)                           //玄慈和灭绝显然不是同一个人
        pre[fx]=fy;                        //方丈只好委委屈屈地当了师太的手下啦
}
```

#### **作用：**

**1.统计联通块数量**

```cpp
for (int i = 0; i < n; i++)
{
    if (i == find(i, p))
        cnt++;
}
```

**一个门派只占一个位！**

## 2.处理更新：新的并查集

1.运用vector数组可赋值，直接开一个新的vector临时容器，vector就是牛逼！

```cpp
vector<vector<int>> adj2 = adj;
cnt2 = 0;
for (int i = 0; i < n; i++)
    p[i] = i;
for (int i = 0; i < n; i++)
    adj2[index][i] = adj2[i][index] = 0x3f3f3f3f;
for (int i = 0; i < n; i++)
{
    for (int j = 0; j < n; j++)
    {
        if (adj2[i][j] != 0x3f3f3f3f)
            Union(i, j, p);
    }
}
```



### 3.终于爽辣！ 直接用差值判断

```cpp
if (cnt2 - cnt > 1)
    print2(index);
else
    print1(index);
```

## 完整代码：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;
void print1(int i)
{
    cout << "City " << i << " is lost.\n";
}
void print2(int i)
{
    cout << "Red Alert: City " << i << " is lost!\n";
}

int find(int x, vector<int> p)
{
    if (x == p[x])
        return x;
    return p[x] = find(p[x], p);
}
void Union(int x, int y, vector<int> &p)
{
    int xx = find(x, p);
    int yy = find(y, p);
    if (xx != yy)
        p[xx] = yy;
}
int main()
{

    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    vector<int> p(n);
    for (int i = 0; i < n; i++)
        p[i] = i;
    vector<vector<int>> adj(n, vector<int>(n, 0x3f3f3f3f));
    for (int i = 0; i < m; i++)
    {
        int num1, num2;
        cin >> num1 >> num2;
        adj[num1][num2] = adj[num2][num1] = 1;
        Union(num1, num2, p);
    }
    int cnt = 0, cnt2 = 0;
    for (int i = 0; i < n; i++)
    {
        if (i == find(i, p))
            cnt++;
    }
    int K;
    cin >> K;
    while (K--)
    {
        int index;
        cin >> index;
        vector<vector<int>> adj2 = adj;
        cnt2 = 0;
        for (int i = 0; i < n; i++)
            p[i] = i;
        for (int i = 0; i < n; i++)
            adj2[index][i] = adj2[i][index] = 0x3f3f3f3f;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (adj2[i][j] != 0x3f3f3f3f)
                    Union(i, j, p);
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (i == find(i, p))
                cnt2++;
        }
        if (cnt2 - cnt > 1)
            print2(index);
        else
            print1(index);
    }
    cout << "Game Over.\n";
    return 0;
}
```



PPT上的并查集：根不是自己，而是无根（-1）

<img src="https://liwenjunpictures.oss-cn-shenzhen.aliyuncs.com/202411082304565.png" alt="image-20241108230407488" style="zoom:67%;" />

