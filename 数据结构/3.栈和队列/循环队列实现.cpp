// UTF-8
// 循环队列
#include <iostream>
using namespace std;

class SQueue
{
    int *base;
    int frone, rear; // 头指针，尾指针
    int MaxSize;     // 最大容量
public:
    SQueue(int n);
    void push(int e); // 元素e入队列
    void pop();       // 出队列
    int front();      // 返回队头元素
    bool full();      // 判队满
    bool empty();     // 判队空
};

void SQueue::push(int e)
{
    base[rear] = e;
    rear = (rear + 1) % MaxSize;
}
int main()
{
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    return 0;
}

