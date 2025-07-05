# 遇到问题
1. .dat文件没有权限
**解决方法：chmod 777 可执行文件**

**ctl+c 可以退出**
**==用来调试可执行文件 gdb bomb_64.dat==**
# 常用命令:

**file [file]	   装入想要调试的可执行程序 file**
attach    将gdb attach到一个运行时的进程 ，并调试它
kill		 终止正在调试的程序
quit		   终止gdb

# 单步跟踪和从断点开始继续执行
  **continue  从断点开始继续执行**
  next  在**不单步执行进入其他函数的情况**下，向前执行一行源代码
  finish  运行程序，直到当前函数完成返回。并打印函数返回时的堆栈地址和返回值及参数值等信息。
# 获取帮助
**help**
# 设置断点
**break [where]  在代码里设置断点, 这将使程序执行到这里时被挂起.  where 可以16进制地址、函数名、行  号、相对的行位移**
[r]watch expr  为表达式（变量） expr 设置一个观察点。一旦表达式值被写或读时，马上停住程序
info break[points]  列出当前所有的断点
clear [where]  去除一个指定的断点
break explode_bomb
break phase_1

# 开始运行
run
run [args]	   运行选择的程序并将参数args传给它
# 检查汇编 会给出对应的代码的汇编
**==disas==** 

# 查看寄存器内容
list [where]  列出产生执行文件的源代码部分 where
search regexp在源文件中搜索正规表达式 regexp
backtrace [n]  显示程序中的当前位置和表示如何到达当前位置的栈跟踪 n
info [what]  显示与该程序有关的各种信息 what (可以是局部变量和函数参数)
	info breakpoints/threads/locals……
p[rint] [expr]  打印变量或是表达式的值 expr
**info registers**


# 打印指定寄存器
print $rsp

# 每步执行
stepi
step  执行下一个源程序行，必要时进入下一个函数
# 检查寄存器或某个地址
x/4wd $rsp
x $esi 检查寄存器的值