// vscode, UTF-8
// OJ顺序表题目静态链表实现。

#include <iostream>
using namespace std;

#define SIZE 1000

// 链表结点
struct node
{
    int data; // 数据域
    int next; // 游标
};

// 链表类型定义
typedef node slist[SIZE];

// 初始化l的备用链表和静态链表头结点
void init(slist l)
{
    int i;
    for (i = 0; i < SIZE; i++) // 初始，都是备用链表结点
        l[i].next = i + 1;
    l[SIZE - 1].next = 0;
    l[1].next = 0; // 静态链表头结点
    l[0].next = 2; // 备用链表头结点
}

// 查找静态链表第i个结点下标
int getnode(slist l, int i)
{
    int p, k;
    if (i < 0) // 头结点1是第0个结点，小于0，不存在
        return 0;

    // 依次计数结点k
    for (p = 1, k = 0; p && k < i; k++, p = l[p].next)
        ;
    return p; // p值0，不存在第i个结点；否则，存在
}

// 静态链表第i个结点插入元素e
int insert(slist l, int i, int e)
{
    int p, k;
    p = getnode(l, i - 1); // 查找第loc-1个结点

    if (p) // 找到，假设有备用结点
    {
        k = l[0].next;         // 取第一个可用结点，下标k
        l[0].next = l[k].next; // 从备用链表删除k结点

        l[k].data = e;         // 设置元素
        l[k].next = l[p].next; // 插在p之后
        l[p].next = k;         // p之后是k结点
        return 1;
    }
    return 0; // 未找到第i-1个结点
}

// 静态链表删除第i个结点
int listdelete(slist l, int i)
{
    int p, q;
    p = getnode(l, i - 1); // 查找第loc-1个结点

    if (p && l[p].next) // 存在第i-1和第i个结点
    {
        q = l[p].next;         // 第i个结点
        l[p].next = l[q].next; // 删除第i个结点

        l[q].next = l[0].next; // 头插插入备用链表中
        l[0].next = q;
        return 1;
    }
    return 0; // 删除不成功
}

// 计算静态链表长度
int getlength(const slist l)
{
    int k, p;

    k = 0; // 第0个结点
    p = 1; // 静态链表头结点

    while (l[p].next) // 若存在下一个结点
    {
        k++; // 移动
        p = l[p].next;
    }
    return k; // 返回结果
}

// 输出静态链表
void display(const slist l)
{
    int p;

    cout << getlength(l) << " "; // 元素数
    p = l[1].next;               // 第一个结点
    while (p)                    // p结点存在
    {
        cout << l[p].data << " "; // 输出值
        p = l[p].next;            // 后移
    }
    cout << endl;
}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    int n, i, e, loc;

    slist l; // 静态链表l
    init(l); // 初始化备用链表和静态链表头结点

    cin >> n; // 依次插入n个元素
    for (i = 0; i < n; i++)
    {
        cin >> e;
        insert(l, i + 1, e);
    }
    display(l); // 输出链表

    for (i = 0; i < 2; i++) // 输入位置，元素，插入链表
    {
        cin >> loc >> e;
        if (insert(l, loc, e))
            display(l);
        else
            cout << "error" << endl;
    }

    for (i = 0; i < 2; i++) // 输入位置，删除结点
    {
        cin >> loc;
        if (listdelete(l, loc))
            display(l);
        else
            cout << "error" << endl;
    }

    for (i = 0; i < 2; i++) // 输入位置，查找结点
    {
        cin >> loc;
        int p = getnode(l, loc);
        if (p != 1 && p) // 非头结点且存在
            cout << l[p].data << endl;
        else
            cout << "error" << endl;
    }
    return 0;
}
