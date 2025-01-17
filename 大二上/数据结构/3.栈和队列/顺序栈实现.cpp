// vscode, UTF-8
// 栈实现
#include <iostream>
#include <stack>
using namespace std;

// 栈类
class cstack
{
    int *base;   // 栈空间基址
    int itop;    // 栈顶，为和函数top区分
    int MaxSize; // 最大栈容量
public:
    cstack(int n);    // 分空间，初始化MaxSize，栈顶
    void push(int e); // 入栈
    int top();        // 返回栈顶元素
    void pop();       // 出栈
    bool empty();     // 判栈空
    bool isfull();    // 判栈满
    int size();       // 返回栈中元素数
    ~cstack();        // 析构释放空间
};

// 构造函数
cstack::cstack(int n) : itop(0), MaxSize(n)
{
    base = new int[MaxSize];
}

// 入栈，先写入itop位置，itop+1
void cstack::push(int e)
{
    base[itop++] = e;
}

// 返回栈顶元素（itop-1位置元素），不出栈
int cstack::top()
{
    return base[itop - 1];
}

// 出栈
void cstack::pop()
{
    itop--;
}

// 判栈空，返回true或false
bool cstack::empty()
{
    return itop == 0;
}

// 判栈满，返回true或false
bool cstack::isfull()
{
    return itop == MaxSize;
}

// 返回栈中元素数
int cstack::size()
{
    return itop;
}

// 析构释放空间
cstack::~cstack()
{
    delete[] base;
}
int main()
{
    int t;
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    //cstack s(10);
    stack<int> s;   //使用STL的栈，无栈满

    int e;
    for (int i = 0; i < 3; i++)
    {
        cin >> e;
        s.push(e);
    }

    while (!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;

    return 0;
}


/*测试数据
10 20 30
*/