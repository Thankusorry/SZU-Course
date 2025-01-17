#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Vector {
  int *data;
  int size, length;
}Vector;


//顺序表初始化操作
Vector* init(int n) {

  Vector *vec = (Vector*)malloc(sizeof(Vector) );
  if (vec == NULL)//if (p == NULL || i == NULL)
  {
    printf("Initialization failed");
    return 0;
  }
  vec->data = (int*)malloc(sizeof(int) * n);
  vec->size = n;
  vec->length = 0;
  return vec;
}

//扩容
int expand(Vector* vec) {
 /* 
 vec->size *= 2;//扩容两倍
 vec->data = (int*)realloc(vec->data, sizeof(int) * vec->size);
 // 有bug，realloc失败后会返回一个空地址，覆盖掉原来vec->data的地址，
  造成内存泄漏
  */
  int new_size = vec->size * 2;
  int *p = (int*)realloc(vec->data, sizeof(int) * new_size);
  if (p == NULL) return 0;
  vec->size = new_size;
  vec->data = p;
  return 1;
}

//插入
int insert(Vector* vec, int ind, int val) {
  //非法操作
  if (vec == NULL)return 0;
  //扩容操作
  if (vec->length == vec->size) {
    if (!expand(vec))return 0;
    printf("expand vector size to %d success\n", vec->size);
  }
  if (ind < 0 || ind >vec->length)return 0;
  
  

  //合法操作
  //错误写法,会覆盖后面的元素，正确应该倒着写
  /*for (int i = ind; i < vec->length; i++)
  {
    vec->data[i + 1] = vec->data[i];
  }*/

  for (int i = vec->length ; i > ind; i--)
  {
    vec->data[i] = vec->data[i - 1];
  }

  //将插入值放入ind位
  vec->data[ind] = val;
  vec->length += 1;
  return 1;
}


//删除
int erase(Vector* vec, int ind) {
  if (vec == NULL)return 0;
  if (vec->length == 0)return 0;
  if (ind < 0 || ind >= vec->length)return 0;
  
  for (int i = ind + 1; i < vec->length; i++)
  {
    vec->data[i - 1] = vec->data[i];
  }
  vec->length -= 1;
  return 1;
}

//顺序表销毁操作
void clear(Vector* vec)
{
  if (vec == NULL)return;
  free(vec->data);
  free(vec);
  return;
}

void output(Vector* vec) {
  printf("array(%d) = [", vec->length);
  for (int i = 0; i < vec->length; i++)
  {
    //为了美观向前输出一个逗号
    if (i != 0)printf(", ");
    printf("%d", vec->data[i]);
  }
  printf("]\n");
  return ;
}

int main()
{
  srand(time(0));
#define MAX_OP 20
  Vector* vec = init(1);
  int op, ind, val;
  for (int i = 0; i < MAX_OP; i++)
  {
    /*50%生成插入 50%生成删除；
    若op = rand() % 4，case 0,case 2. case 3一类，75%；
    case 1一类25%*/
    op = rand() % 2;
    ind = rand() % (vec->length + 1);
    val = rand() % 100;
    switch (op)
    {
      case 0: {
        printf("insert %d at %d to vector = %d\n", 
          val, ind, insert(vec, ind, val));
      }break;
      case 1:
      {
        printf("erase item at %d from vector = %d\n", 
        ind, erase(vec, ind));
      }break;
    }
    output(vec);
    printf("\n");
  }
  return 0;
}
