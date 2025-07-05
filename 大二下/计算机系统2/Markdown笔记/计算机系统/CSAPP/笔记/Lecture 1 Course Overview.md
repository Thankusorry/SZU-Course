
### Instructors
**Randal E. Bryant and David R. O'Hallaron**

### Purpose
 To give you enough understanding of what ==that box== is doing when it executes your code.
### Reality #1
**Ints are not Integers,Floats are not Reals.**
1. 整数会溢出
2. 浮点数一个特别大的数加一个很小的数不会改变运算结果-> 不符合结合律  <font color="#f79646">Round off (取舍问题)</font>

It all comes down to the fact that ==they use finite representations of things that are potentially infinite in their expanse==

![[Pasted image 20250208154012.png]]

#### Cannot assume all "usual" mathematical properties.
- Due to finiteness of representations
- Integer operations satisfy "ring" properties  整数运算是一个环
	-  Commutativity,associativity,distributivity 交换律，结合律，分配律
- ==Floating point（浮点数）== operations satisfy "ordering" properties **排序**性质
	-  Monotonicity（ 单调性）,values of signs 

### Reality #2 
You've Got to Know ==Assembly==

### Reality # 3
Memory matters 

- Momory is not unbounded (无穷的)
	- It must be allocated(分配的) and managed
	- Many applications are ==memory dominated== 受内存限制
- ==Memory referencing== bugs especially pernicious  内存引用bug 非常致命
- Memory performance is not uniform 
	- Cache and virtual memory effects

#### Memory Referencing Bug Example
```
typedef struct {

  int a[2];

  double d;

} struct_t;

double fun(int i) {

  volatile struct_t s;

  s.d = 3.14;

  s.a[i] = 1073741824; /* Possibly out of bounds */

  return s.d;
}
```

```
fun(0)  ➙  3.14

fun(1)  ➙  3.14

fun(2)  ➙  3.1399998664856

fun(3)  ➙  2.00000061035156

fun(4)  ➙  3.14

fun(6)  ➙  Segmentation fault
```

#### Explanation:
![[Pasted image 20250208162220.png]]

图示结构内存一行四个字节（int 四个字节，double八个字节） **It doesn't do bonud check**  

### Reality #4 Performance
Must optimize(优化)  at multiple levels.

How to improve performance without destroying code modularity and generality. (模块性和通用性)

![[Pasted image 20250208210702.png]]
和其他课程的联系
![[Pasted image 20250208211240.png]]


