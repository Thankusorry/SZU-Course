#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Stack {
  int* data;
  int top, size;
}Stack;


//初始化栈
Stack* init(int n) {
  Stack* s = (Stack*)malloc(sizeof(Stack));
  if (s == NULL)return 0;
  s->data = (int*)malloc(sizeof(int) * n);
  s->size = n;
  s->top = -1;
  return s;
}

//栈的判空操作
int empty(Stack* s) {
  return s->top == 1;
}

//查看栈顶元素操作
int top(Stack* s) {
  if (empty(s))return 0;
  return s->data[s->top];
}

//入栈操作
int push(Stack* s, int val) {
  if (s == NULL)return 0;
  if (s->top + 1 == s->size)return 0;
  s->top += 1;
  s->data[s->top] = val;
  return 1;
}

//出栈操作
int pop(Stack* s) {
  if (s == NULL)return 0;
  if (empty(s))return 0;
  s->top -= 1;
  return 1;
}

//栈的清空操作
void clear(Stack* s) {
  if (s == NULL)return;
  free(s->data);
  free(s);
  return;
}

//输出
void output(Stack* s) {
  printf("stack(%d) = [",s->top + 1);
  //倒着遍历
  for (int i = s->top; i >= 0; i--)
  {
    printf(" %d", s->data[i]);
  }
  printf("]\n");
  return;
}

void main() {
  srand((unsigned)time(0));
#define MAX_OP 20
  Stack* s = init(MAX_OP);
  for (int i = 0; i < MAX_OP; i++)
  {
    int op = rand() % 2, val = rand() % 100;
    switch (op){
    case 0: {
      printf("push %d to stack = %d\n", val, push(s, val));
    }break;
    case 1: {
      int top1 = top(s);
      int pop1 = pop(s);
      printf("pop %d from stack = %d\n", top1, pop1);//从右往左计算
    }break;
    }
    output(s);
  }
  return;
}