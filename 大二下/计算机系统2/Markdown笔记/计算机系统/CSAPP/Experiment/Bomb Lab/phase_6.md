```asm
000000000040106f <fun6>:
初始状态: 
%rdi = 链表头节点地址 (0x602780)
(链表节点结构: [val, next_ptr])
  40106f:   4c 8b 47 08           mov    0x8(%rdi),%r8     ; r8 = head->next (保存原链表第二个节点)
  401073:   48 c7 47 08 00 00 00  movq   $0x0,0x8(%rdi)    ; head->next = NULL (断开原链表)
  40107a:   00 
  40107b:   48 89 f8              mov    %rdi,%rax         ; rax = head (最终返回排序后的头)
  40107e:   48 89 f9              mov    %rdi,%rcx         ; rcx = head (当前遍历指针)
  
  401081:   4d 85 c0              test   %r8,%r8           ; 检查 r8 (原链表剩余节点) 是否为空
  401084:   75 40                 jne    4010c6 <fun6+0x57> ; 不为空，进入主循环
  
  ; 若原链表只有头节点，直接返回
  401086:   48 89 f8              mov    %rdi,%rax         ; 返回值 = head
  401089:   c3                    retq                     

; ---- 主循环开始 (处理 r8 指向的节点) ----
; 此时:
;   r8 = 待插入节点 (原链表未处理部分)
;   rcx = 已排序链表的当前遍历指针
;   rdi/rax = 已排序链表的头

; 🧠 阶段1: 寻找插入位置 (比较当前节点值)
40108d:   48 8b 51 08           mov    0x8(%rcx),%rdx    ; rdx = curr->next
401091:   48 85 d2              test   %rdx,%rdx          ; 检查是否到达已排序链表末尾
401094:   74 09                 je     40109f <fun6+0x30> ; 如果 curr->next == NULL，跳转

; 比较当前节点的「下一个节点值」与「待插入节点值」
401096:   39 32                 cmp    %esi,(%rdx)        ; 比较 curr->next->val 与 r8->val (此时 esi=r8->val)
401098:   7f f0                 jg     40108a <fun6+0x1b> ; 若 curr->next->val > r8->val，继续向后查找

; 找到插入位置: curr->next->val <= r8->val 或 curr->next 为空
40109a:   48 89 cf              mov    %rcx,%rdi          ; rdi = curr (记录插入位置前驱)
40109d:   eb 03                 jmp    4010a2 <fun6+0x33> ; 进入阶段2

; 处理已排序链表末尾的情况
40109f:   48 89 cf              mov    %rcx,%rdi          ; rdi = curr (插入到末尾)

; 🧠 阶段2: 插入节点到正确位置
4010a2:   48 39 d7              cmp    %rdx,%rdi          ; 检查插入位置是否在末尾 (rdx=curr->next)
4010a5:   74 06                 je     4010ad <fun6+0x3e> ; 如果是末尾，跳转

; 常规插入操作: 将 r8 插入到 rdi 和 rdx 之间
4010a7:   4c 89 47 08           mov    %r8,0x8(%rdi)     ; rdi->next = r8
4010ab:   eb 03                 jmp    4010b0 <fun6+0x41>

; 处理插入到末尾的情况
4010ad:   4c 89 c0              mov    %r8,%rax           ; rax = r8 (更新返回头节点)

; 重新链接指针，继续处理下一个节点
4010b0:   49 8b 48 08           mov    0x8(%r8),%rcx      ; rcx = r8->next (保存下一个待处理节点)
4010b4:   49 89 50 08           mov    %rdx,0x8(%r8)      ; r8->next = rdx (连接后续节点)
4010b8:   48 85 c9              test   %rcx,%rcx          ; 检查是否还有未处理节点
4010bb:   74 1a                 je     4010d7 <fun6+0x68> ; 若无，结束循环
4010bd:   49 89 c8              mov    %rcx,%r8           ; r8 = 下一个待处理节点
4010c0:   48 89 c1              mov    %rax,%rcx          ; rcx = 已排序链表头 (重置遍历指针)
4010c3:   48 89 c7              mov    %rax,%rdi          ; rdi = 已排序链表头

; 🧠 下一轮循环准备
4010c6:   48 89 ca              mov    %rcx,%rdx          ; rdx = 当前遍历指针
4010c9:   48 85 c9              test   %rcx,%rcx          ; 检查当前指针是否为空
4010cc:   74 d4                 je     4010a2 <fun6+0x33> ; 若为空，直接插入

4010ce:   41 8b 30              mov    (%r8),%esi         ; esi = r8->val (待插入节点的值)
4010d1:   39 31                 cmp    %esi,(%rcx)        ; 比较 curr->val 与 r8->val
4010d3:   7f b8                 jg     40108d <fun6+0x1e> ; 若 curr->val > r8->val，继续向后查找
4010d5:   eb cb                 jmp    4010a2 <fun6+0x33> ; 否则进入插入阶段

4010d7:   f3 c3                 repz retq                 ; 返回排序后的链表头
```
