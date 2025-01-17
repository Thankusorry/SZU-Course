// vscode, UTF-8
// 带头结点单链表插入、删除
#include <iostream>
using namespace std;

// 结点结构体
struct LNode
{
    int data;    // 数据域
    LNode *next; // 指针域
    LNode(int e = 0, LNode *nextval = nullptr) : data(e), next(nextval) {}
};
typedef LNode *LinkList; // LinkList是结点指针类型的别名

// 初始化生成头结点
void initlist(LinkList &head) // head链表头指针，修改值，传引用
{
    head = new LNode;
}

// 返回结点个数
int getlength(LinkList head)
{
    LNode *p = head; // 头结点
    int k = 0;       // k计数p结点位置，初始第0个结点
    while (p->next)  // 若存在下一个结点
    {
        p = p->next; // 后移
        k++;         // 计数加1
    }
    return k;
}

// 查找第i个结点，并返回其指针
LNode *getelem(LinkList head, int i)
{
    if (i < 0) // 结点序号从0开始，头结点是第0个结点
        return nullptr;
    LNode *p = head;   // 头结点
    int k = 0;         // k计数p结点位置，初始第0个结点
    while (p && k < i) // 移动p和k
    {
        p = p->next;
        k++;
    }
    return p; // 若p为空指针，不存在第i个结点；否则存在
}

// 第i个位置插入元素e(i从1开始)
int listinsert(LinkList head, int i, int e)
{
    LNode *p = getelem(head, i - 1); // 查找第i-1个结点p

    if (!p) // 第i-1个结点不存在,不能插入
        return 0;

    // 创建新结点插入
    LNode *s = new LNode(e, p->next);
    p->next = s;
    return 1;
}

// 删除第i个结点（i从1开始）
int listdelete(LinkList head, int i)
{
    LNode *p = getelem(head, i - 1); // 查找第i-1个结点p

    // 补齐代码

    return 1;
}

// 输出单链表
void listdisplay(LinkList head)
{
    LNode *p = head->next; // p指向第一个表结点

    cout << getlength(head) << " ";
    while (p) // p非空
    {
        cout << p->data << " "; // 输出结点数据
        p = p->next;            // p后移，指向下一个结点
    }
    cout << endl;
}

// 释放单链表
void listfree(LinkList &head)
{
    LNode *p, *q;

    for (p = head; p;) // p初始指向头结点,p非空循环
    {
        q = p;       // q指向p结点
        p = p->next; // p后移
        delete q;    // 释放q,注意语句顺序
    }
    head = nullptr;
}

// 尾插创建含n个元素的单链表
void createlist(LinkList head, int n, int num[])
{
    LNode *tail = head; // 初始头结点即最后一个结点
    for (int i = 0; i < n; i++)
    {
        LNode *s = new LNode(num[i]); // 创建结点s,next为空
        tail->next = s;               // 链接到tail之后
        tail = s;                     // s结点是当前链表最后一个结点
    }
}

// la,lb有序表合并
LinkList mergelist(LinkList la, LinkList lb)
{
    LinkList lc;  // 结果链表头指针
    initlist(lc); // 生成头结点
    LNode *pa = la->next;
    LNode *pb = lb->next;
    LNode *pc = lc->next; // 结果链表尾插

    while (pa && pb)
    {
        if (pa->data <= pb->data) // pc链接pa结点
        {
            pc->next = pa; // 也可以new结点，不修改原链表
            pc = pc->next;
            pa = pa->next;
        }
        else // pc链接pb结点
        {
            // 略
        }
    }
    // 两个链表中的剩余结点挂在pc末尾
    pc->next = pa ? pa : pb ? pb
                            : nullptr;
    la->next = nullptr;
    lb->next = nullptr;
    return lc;
}

int main()
{
// 文件重定向，查用法
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    LinkList head; // 定义单链表l
    initlist(head);
    int n, num[1000], loc, elem;

    cin >> n; // 初始数据输入
    for (size_t i = 0; i < n; i++)
        cin >> num[i];

    createlist(head, n, num); // 尾插法创建单链表
    listdisplay(head);
    for (int i = 0; i < 2; i++)
    {
        cin >> loc >> elem;
        if (!listinsert(head, loc, elem)) // 插入
            cout << "error" << endl;
        else
            listdisplay(head);
    }
    return 0;
}
