反汇编 可执行目标文件.exe和可重定位目标文件.o的区别：
`objdump -d main.o` or `objdump -d main`
1. 地址表示：.o文件地址从0开始的块内偏移，执行程序是绝对的虚拟地址
2. 符号和重定位：
   .o文件保留所有未解析信息，执行程序符号被解析为实际地址