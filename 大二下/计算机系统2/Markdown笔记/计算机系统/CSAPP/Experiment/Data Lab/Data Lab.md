  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations **! ~**
  4. Binary integer operations **& ^ | + << >>**


**You are expressly forbidden to:**
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  **You may assume that your machine:**
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.

## Content
根据bits.c中的要求补全以下的函数：

[[1.int bitXor(int x, int y);]]

[[2.int tmin(void);]]

[[3.int isTmax(int x);]]

[[4.int allOddBits(int x);]]

[[5.int negate(int x);]]

[[6.int isAsciiDigit(int x);]]

[[7.int conditional(int x, int y, int z);]]

[[8.int isLessOrEqual(int x, int y);]]

[[9.int logicalNeg(int x);]]

[[10.int howManyBits(int x);]]

[[11.unsigned floatScale2(unsigned uf);]]

[[12.int floatFloat2Int(unsigned uf);]]

[[13.unsigned floatPower2(int x);]]