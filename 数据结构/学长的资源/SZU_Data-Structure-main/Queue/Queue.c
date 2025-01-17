#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Queue {
  int* data;
  int head, tail;
  int length;
  int cnt;//当前队列中存储元素的总数量
}Queue;

//初始化队列
Queue* init(int n) {
  Queue* q = (Queue*)malloc(sizeof(Queue));
  if (q == NULL)return 0;
  q->data = (int*)malloc(sizeof(int) * n);
  q->head = q->tail = q->cnt = 0;
  q->length = n;
  return q;
}

//队列判空操作
int empty(Queue* q) {
  //return q->head == q->tail;//头指针等于尾指针,非循环
  return q->cnt == 0;
}


//查看队首元素
int front(Queue* q) {
  if (q->head == q->tail) return 0;
  return q->data[q->head];
}

//入队操作
int push(Queue* q, int val) {
  if (q == NULL)return 0;
  //if (q->tail == q->length)return 0;//判断队列是否已满,假溢出
  if (q->cnt == q->length)return 0;//判断队列是否已满
  q->data[q->tail++] = val;
  if (q->tail >= q->length)q->tail -= q->length;
  q->cnt += 1;
  return 1;//入队成功
}

//出队操作
int pop(Queue* q) {
  if (q == NULL)return 0;
  if (empty(q))return 0;
  q->head++;
  if (q->head == q->length)q->head -= q->length;
  q->cnt -= 1;
  return 1;
}
//队列的销毁
void clear(Queue* q) {
  if (q == NULL)return;
  free(q->data);
  free(q);
  return;
}

//输出队列操作
void output(Queue *q) {
  printf("queue = [");
  //for (int i = q ->head;i < q->tail; i++)
  for (int i = q->head, j = 0; j < q->cnt; j++)
  {
    //printf("%d ", q->data[i]);
    int ind = (i + j) % q->length;
    printf(" %d", q->data[ind]);
  }
  printf("]\n\n");
}

int main() {
  srand((unsigned)time(0));
#define MAX_OP 20
  Queue* q = init(MAX_OP);
  for (int i = 0; i < MAX_OP; i++)
  {
    int val = rand() % 100, op = rand() % 5;//push:pop=3:2
    switch (op)
    {
    case 0:
    case 3:
    case 4:{
      printf("push %d to queue = %d\n", val, push(q, val));
      }break;
    case 1:
    case 2: {
      //printf("pop %d from queue = %d\n", front(q), pop(q));//会出现-842150451：未定义结果
      //即数组越界、或使用未初始化的单元
      /*
      在windows环境下gcc编译器printf函数执行的时候，
      参数计算顺序是从右到左的，这样就导致先执行出队操作，
      再获取队首元素，导致最后打印出来的pop元素是出队前的
      第二个元素而不是队首元素
      法一：
      printf("queue = %d,pop %d from queue\n", pop(q), front(q));
      */
      //法二：
      int front1 = front(q);
      int pop1 = pop(q);
      printf("pop %d from queue = %d\n", front1, pop1);
    }break;
    }
    output(q);
  }
  printf("head = %d, tail = %d, count = %d\n", q->head, q->tail, q->cnt);
  for (int i = 0; i < MAX_OP; i++)
  {
    int val = rand() % 100;
    printf("push %d to queue = %d\n", val, push(q, val));
  }
  output(q);
  return 0;
}