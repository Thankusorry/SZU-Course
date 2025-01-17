#include <stdio.h>
#include <string.h>


void addBinary(char *a, char *b)
{

   int lenA = strlen(a);
   int lenB = strlen(b);
   int maxLen = lenA > lenB ? lenA : lenB;
   char result[1001]; // 结果字符串，长度为 maxLen + 1
   int carry = 0;     // 进位
   int index = 0;     // 结果字符串的索引

   // 从右到左逐位相加
   for (int i = lenA - 1, j = lenB - 1; i >= 0 || j >= 0 || carry; i--, j--)
   {
      int digitA = (i >= 0) ? a[i] - '0' : 0;
      int digitB = (j >= 0) ? b[j] - '0' : 0;
      int sum = digitA + digitB + carry;
      // 计算当前位的值和进位
      result[index++] = (sum % 2) + '0';
      carry = sum / 2;
   }
   
   // 结果字符串是反向的，需要反转
   for (int i = 0; i < index / 2; i++)
   {
      char temp = result[i];
      result[i] = result[index - 1 - i];
      result[index - 1 - i] = temp;
   }

   result[index] = '\0'; // 添加字符串结束符

   // 输出结果
   printf("%s\n", result);
}

int main()
{
   char a[1000], b[1000];

   // 读取多组测试数据
   while (scanf("%s %s", a, b) != EOF)
   {
      addBinary(a, b);
   }
   return 0;
}

