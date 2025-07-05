查看main函数，找到phase_1相关信息

%rax的值赋给第一个参数%rdi
**disas 查看汇编代码**
**info registers 查看寄存器的值**

关键： 

**==400e70：==**
sub $8, %rsp
**mov $0x401af8 , %esi**
	**%rsi第二个参数**
	输出$0x401af8：**"Science isn't about why, it's about why not?"**
通过info registers 得到 **%rdi =0x602f40**
	查看对应字符串 ： "\nBOOM!!!" 这是爆炸后输出的结果
**call 40123d <strings_not_equal>**
	结果存在%rax
test %eax,%eax
je   400e87 <phase_1+0x17>
	**eax等于0跳转**
**call 40163d <explode_bomb>**
	 **如果%rax不能与0，引爆炸弹**
add $8,%rsp
	栈指针复原
ret

推测：这个函数是判断两个字符串是否相等，需要具体验证
**==40123d：==**
保存寄存器：r12(8) -> rbp(2) -> rbx(8)
mov %rbx, -0x18(%rsp) 
mov %rbp,-0x10(%rsp)
mov %r12,-0x8(%rsp)

sub $0x18,%rsp
mov %rdi,%rbx  
	**%rbx=%rdi**
mov %rsi,%rbp
	**%rbp=$0x401af8**
**call 401221**
mov $1,%edx
cmp %eax,%r12d
**jne 4012a6**

**==401221 <string_length>==**
mov $0,%eax
**%rax=0**
cmpb $0,(%rdi)
	M[%rdi] 
je **40123b** 
	M[%rdi] == 0
mov %rdi,%rdx
**40122e** add $0x1,%rdx
mov %edx,%eax
sub %edi,%eax
cmpb $0,(%rdx)
**jne 40122e**
 **40123b ：repz ret**


 **4012a6**： add $0x18,%rsp
 ret