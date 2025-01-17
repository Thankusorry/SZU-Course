// vscode, UTF-8
// 带头结点单链表插入、删除
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
    LNode *head;
    int getlength();       // 计算结点个数
    LNode *getelem(int i); // 查找第i个结点，并返回其指针
    friend LinkList &mergelist(LinkList &la, LinkList &lb);  //la,lb有序单链表合并
public:
    LinkList();                   // 初始化
    int listinsert(int i, int e); // 第i个位置插入元素e
    int listdelete(int i);        // 删除第i个结点
    void listdisplay();           // 输出单链表
    void createlist(int n, int num[]); //创建含n个元素的单链表
    ~LinkList();                  // 析构释放链表
};

// 结点类构造函数
LNode::LNode(int e, LNode *nextval) : data(e), next(nextval)
{
}

// 结点类访问接口
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

// 链表类构造函数,生成头结点
LinkList::LinkList()
{
    head = new LNode;
}

//返回结点个数
int LinkList::getlength()
{
    LNode *p = head;   //头结点
    int k = 0;         //k计数p结点位置，初始第0个结点
    while(p->getnext())   //若存在下一个结点
    {
        p = p->getnext();   //后移
        k++;                //计数加1
    }
    return k;
}

// 查找第i个结点，并返回其指针
LNode *LinkList::getelem(int i)
{
    if (i < 0)      //结点序号从0开始，头结点是第0个结点
        return nullptr;
    LNode *p = head;   //头结点
    int k = 0;         //k计数p结点位置，初始第0个结点
    while(p && k<i)    //移动p和k
    {
        p = p->getnext();
        k++;
    }
    return p;        //若p为空指针，不存在第i个结点；否则存在
}

//第i个位置插入元素e(i从1开始)
int LinkList::listinsert(int i, int e)
{
    LNode *p = getelem(i-1);     //查找第i-1个结点p

    if(!p)      //第i-1个结点不存在,不能插入
        return 0;

    //创建新结点插入
    LNode *s = new LNode(e,p->getnext());   
    p->setnext(s);
    return 1;
}

// 删除第i个结点（i从1开始）
int LinkList::listdelete(int i)
{
    LNode *p = getelem(i-1);     //查找第i-1个结点p
    
    //补齐代码

    return 1;
}
    

//输出单链表
void LinkList::listdisplay()
{
    LNode *p = head->getnext();   //p指向第一个表结点

    cout << getlength() << " ";
    while(p)    //p非空
    {
        cout << p->getdata() << " ";  //输出结点数据
        p = p->getnext();         //p后移，指向下一个结点
    }
    cout << endl;
}

// 析构函数释放单链表
LinkList::~LinkList()
{
    LNode *p, *q;

    for(p = head; p; )    //p初始指向头结点,p非空循环 
    {
        q = p;   //q指向p结点
        p = p->getnext();   //p后移
        delete q;    //释放q,注意语句顺序
    }
    head = nullptr;
}

//头插创建含n个元素的单链表
/*void LinkList::createlist(int n, int num[])
{
    for(int i=n-1; i>=0; i--)
    {
        LNode *s = new LNode(num[i],head->getnext());
        head->setnext(s);
    }
}*/

//尾插创建含n个元素的单链表
void LinkList::createlist(int n, int num[])
{
    LNode *tail = head;    //初始头结点即最后一个结点
    for(int i=0; i<n; i++)
    {
        LNode *s = new LNode(num[i]);   //创建结点s,next为空
        tail->setnext(s);    //链接到tail之后
        tail = s;            //s结点是当前链表最后一个结点
    }
}

//la,lb有序表合并
LinkList &mergelist(LinkList &la, LinkList &lb)
{
    static LinkList lc;    //结果链表,返回引用避免调拷贝构造
    LNode *pa = la.head->getnext();
    LNode *pb = lb.head->getnext();
    LNode *pc = lc.head;   //结果链表尾插

    while(pa && pb)
    {
        if(pa->getdata() <= pb->getdata())   //pc链接pa结点
        {
            pc->setnext(pa);    //也可以new结点，不修改原链表
            pc = pc->getnext(); 
            pa = pa->getnext();
        }
        else    //pc链接pb结点
        {
            //略  
        }
    }
    //两个链表中的剩余结点挂在pc末尾
    pc->setnext(pa ? pa : pb? pb: nullptr);
    la.head->setnext(nullptr);
    lb.head->setnext(nullptr);
    return lc;
}

int main()
{
    //文件重定向，查用法
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

    LinkList l;      //定义单链表l
    int n, num[1000],loc,elem;

    cin >> n;        //初始数据输入
    for(size_t i=0; i<n; i++)
        cin >> num[i];

    l.createlist(n,num);     //尾插法创建单链表
    l.listdisplay();
    for(int i=0; i<2; i++)
    {
        cin >> loc >> elem;
        if(!l.listinsert(loc,elem))   //插入
            cout << "error" << endl;
        else
            l.listdisplay();
    }
    return 0;
}
