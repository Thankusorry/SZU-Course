// vscode, utf-8
// 不带头结点单链表插入
#include <iostream>
using namespace std;

class LNode // 结点类
{
private:
    int data;    // 数据域
    LNode *next; // 指针域
public:
    LNode(int e = 0, LNode *nextval = nullptr);
    LNode *getnext(); // 私有数据成员访问接口
    void setnext(LNode *nextval);
    int getdata();
    void setdata(int e);
};

class LinkList // 链表类
{
private:
    LNode *head; // 头指针
public:
    LinkList();                   // 初始化
    int listinsert(int i, int e); // 第i个位置插入元素e
    void listdisplay();           // 输出单链表
    ~LinkList();                  // 析构释放链表
};

// 构造函数
LNode::LNode(int e, LNode *nextval) : data(e), next(nextval)
{
}

// 节点类访问接口
LNode *LNode::getnext()
{
    return next;
}
void LNode::setnext(LNode *nextval)
{
    next = nextval;
}
int LNode::getdata()
{
    return data;
}
void LNode::setdata(int e)
{
    data = e;
}

// 链表类构造函数初始化空表
LinkList::LinkList()
{
    head = nullptr;
}

// 第i个位置插入元素e
int LinkList::listinsert(int i, int e)
{
    LNode *s, *p;
    if (i < 1) // 结点序号从1开始，插入位置不正确
        return 0;

    if (i == 1) // 第一个位置插入，需要修改head，单独处理
    {
        s = new LNode(e, head); // 构建新结点s，指向第一个结点
        head = s;               // 头指针指向新结点s
        return 1;
    }
    else // 非第一个位置插入
    {
        p = head;              // p指向第一个结点
        int k = 1;             // k计数p结点位置，初始1
        while (p && k < i - 1) // 后移p，计数
        {
            p = p->getnext();
            k++;
        }
        if (p) // 若存在第i-1个结点，插入
        {
            s = new LNode(e, p->getnext());
            p->setnext(s);
            return 1;
        }
        return 0; // 第i-1个结点不存在
    }
}

// 遍历单链表
void LinkList::listdisplay()
{
    LNode *p = head; // p指向第一个表结点

    while (p) // p非空
    {
        cout << p->getdata() << " "; // 输出结点数据
        p = p->getnext();            // p后移，指向下一个结点
    }
    cout << endl;
}

// 析构函数释放单链表
LinkList::~LinkList()
{
    LNode *p, *q;

    for (p = head; p;) // p初始指向第一个结点,p非空循环
    {
        q = p;            // q指向p结点
        p = p->getnext(); // p后移
        delete q;         // 释放q,注意语句顺序
    }
    head = nullptr;
}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif

    int n, elem;
    LinkList l;

    // 创建表
    cin >> n; // 元素数
    for (int i = 0; i < n; i++)
    {
        cin >> elem;               // 元素值
        l.listinsert(i + 1, elem); // 按插入链表
    }

    // 输出链表
    l.listdisplay();

    // 测试插入
    if (!l.listinsert(10, 100)) // 位置不对，插入不成功
        cout << "error" << endl;
    else
        l.listdisplay();
}

/*测试数据
5 1 2 3 4 5
*/
