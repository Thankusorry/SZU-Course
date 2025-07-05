//1
/* 
 * bitXor - 仅允许使用~和&来实现异或 
 *   例子: bitXor(4, 5) = 1
 *   允许的操作符: ~ &
 *   最多操作符数目: 14
 *   分值: 1
 */
// A Xor B = (A&~B)U(~A&B)
// AUB= ~(~A&~B)
int bitXor(int x,int y){
    return ~(~(A&~B)&~(~A&B));
}
/* 
 * tmin - 返回最小的二进制补码
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 4
 *   分值: 1
 */
 // Tmin == 10000000...
int tmin(){
    return (1<<31);
}
//2
/*
 * isTmax - 如果x是最大的二进制补码，返回1；否则，返回0
 *   允许的操作符: ! ~ & ^ | +
 *   最多操作符数目: 10
 *   分值: 2
 */
 // Tmax = 011111111
int isTmax(int x) {
    return !(~x^(x+1)) && !!((x+1)^0);
}
/* 
 * allOddBits - 如果所有奇数位都为1则返回1;否则返回0
 *   例子： allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 12
 *   分值: 2
 */
 // 8 16 24  需要7次移位
 //限制255,oxff,一次最多判断8bit,需要判断8次
int allOddBits(int x){
    return  !(((x&(x>>8)&(x>>16)&(x>>24))&0xaa)^0xaa);
}
/* 
 * negate - 返回-x
 *   例子: negate(1) = -1.
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 5
 *   分值: 2
 */
int negate(int x) {
  return ~x+1;
}
//3
/* 
 * isAsciiDigit - 如果x是ascii码中的0~9，返回1;否则返回0
 *   例子: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 15
 *   分值: 3

/* 
 * conditional - 实现x?y:z
 *   例子: conditional(2,4,5) = 4
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 16
 *   分值: 3
 */

/* 
 * isLessOrEqual - 如果x<=y返回1否则返回0
 *   例子: isLessOrEqual(4,5) = 1.
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 24
 *   分值: 3
 */
//4
/* 
 * logicalNeg - 实现！运算符的功能
 *   例子: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   允许的操作符: ~ & ^ | + << >>
 *   最多操作符数目: 12
 *   分值: 4 

/* howManyBits - 返回将X表示为补码所需的最小有效位数。
 *  例子: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  允许的操作符: ! ~ & ^ | + << >>
 *  最多操作符数目: 90
 *  分值: 4
 */

//float
/* 
 * float_twice - 以unsinged表示的浮点数二进制的二倍的二进制unsigned型
 *   参数和结果都会被作为unsigned返回，但是会表示为二进制的单精度浮点值。
 *   允许的操作符: 任何整数或者无符号数操作符包括： ||, &&. also if, while
 *   最多操作符数目: 30
 *   分值: 4
 */

/* 
 * float_i2f - 返回int x的unsigned浮点数的二进制形式
 *   参数和结果都会被作为unsigned返回，但是会表示为二进制的单精度浮点值
 *   允许的操作符: 任何整数或者无符号数操作符包括： ||, &&. also if, while
 *   最多操作符数目: 30
 *   分值: 4
 */

/* 
 * float_f2i - 返回unsigned uf的整型数的二进制形式
 *   参数和结果都会被作为unsigned返回，但是会表示为二进制的单精度浮点值
 *   任何超过范围的数都应该返回 0x80000000u.
 *   允许的操作符: 任何整数或者无符号数操作符包括： ||, &&. also if, while
 *   最多操作符数目: 30
 *   分值: 4
 */
