

7.

##### 如果要读取一个大文件的末尾的一段内容，并且知道该段落的确切位置，最方便的流是：

 A.

FilterStream

 B.

**RandomAccessStream**

 C.

FileStream

 D.

PipedStream





8.

##### 给出一段程序，试判断哪个是正确的输出结果？



public class myprogram



{



​	public static void main (String args[])

​	

​	{

​	

​		try{

​		

​			System.out.print("Hello world"); 

​		

​		}

​		

#### 		finally{

​		

​			System.out.println(" Finally executing"); 

​		

​		}

​	

​	} 



} 



 D.

Hello world Finally executing

( 2.00 分 )



10.

#### 为了实现多线程程序中线程间通讯，需要使用的流是：

 A.

#### PipedStream

 B.

FileStream

 C.

RandomRccessStream

 D.

FilterStream

( 2.00 分 )



11.

##### 下列关于Java源文件的说法中，正确的是（）。

 A.

一个源文件只能有一个方法

 B.

一个源文件只能供一个程序使用

 C.

一个程序可以包括多个源文件

 D.

一个源文件中可以有一个以上的公共类

( 2.00 分 )







1.

##### 已知如下的命令执行 java MyTest a b c 请问哪个语句是正确的？

 A.

args[0] = "a"

 B.

args[0] = "MyTest a b c"

 C.

args[0] = "MyTest"

 D.

args[1]= "b"

( 3.00 分 )

2.

##### 关于this和super关键字，以下代码段正确的是：

 A.

class A{

​	public void methodA(){

​		A test1=this; 

​		test1.methodB();

​	} 

​	public void methodB(){...}

}



 B.

class A{

​	public void methodA(){...}

}

class B extends A{

​	public void methodA(){

​		A test1=super;

​		test1.methodB();

​	}

}



 C.

class A{

​	public void methodA(int i, int j){...}

​	public void methodA(int i){

​		...

​		this.methodA(2,3);

​	}

}



 D.

class A{

​	public A(int i){...}

}

class B extends A{

​	public B(int i,j)

​	{

​		int n=i*j; 

​		super(i);

​		...

​	}

}



 E.

class A

{

​	public void methodA(){...}

}

class B extends A{

​	public void methodA(){

​		super.methodA(); 

​		...

​	}

}



 F.

class A{

​	public A(int i){...}

​	public A(int i, int j)

​	{

​		this(i);

​		...

​	}

}



( 3.00 分 )





4.

已知如下说明：

##### TextArea ta = new TextArea("Hello",5,5); 请问哪个语句是正确的？



 A.

The displayed strings are editable.

 B.

The displayed height is 5 lines otherwise constrain.

 C.

The maximum number of characters in a line is 5.

 D.

The displayed string can use multiple fonts.

( 3.00 分 )





6.

以下哪些选项是正确的？

 A.

String对象中可以存放UNICODE字符。

 B.

String a="Hello World!"; 这样的语句是正确的。

 C.

“+”操作符可用于拼接两个字符串。

 D.

String类就是一个字符串数组。

 E.

String是JAVA中的基本类型，StringBuffer是String的包裹类。

( 3.00 分 )





( 3.00 分 )

8.

##### 设有下面两个赋值语句：  



a = Integer.parseInt("123"); 



b = Integer.valueOf("123").intValue();



 A.

a是整数类对象，b是整数类型变量。

 B.

a是整数类型变量，b是整数类对象。

 C.

a和b都是整数类型变量并且它们的值相等。

 D.

a和b都是整数类对象并且它们的值相等。

( 3.00 分 )

9.

如下哪些字符串是Java中合法的标识符？  

 A.

\#number

 B.

3number

 C.

$number

 D.

fieldname

 E.

super

( 3.00 分 )

10.

下列语句的输出是：



public class test{  



 	public static void main(String []args){ 



​	  A test1=new A();  



​	  A test2=new A();  



​	  A test3=new A(); 



​	  test1.setId();   



​	  System.out.println("test1,  id:"+test1.getId()+"  diff:"+test1.getNextId()); 



​	  test2.setId();       



​	  System.out.println("test2,  id:"+test2.getId()+"  diff:"+test2.getNextId()); 



​	  test3.setId();      



​	  System.out.println("test3,  id:"+test3.getId()+"  diff:"+test3.getNextId()); 



​	  test1.setId();      



​	  System.out.println("test1,  id:"+test1.getId()+"  diff:"+test1.getNextId());  



  } 



} 





class A{  



  public int getId(){ 



  	return id;



  }    



  public void setId(){ 



  	id=base+diff; 



​    diff+=2;    



​    base=id;    



  }   



  public int getNextId(){  



​    return diff;    



  }  



  private int id;  



  private static int diff=1;        



  private static int base=0; 



}



 A.

test1, id:16 diff:9

 B.

test3, id:9 diff:7

 C.

test1, id:9 diff:16

 D.

test1, id:1 diff:1

 E.

test2, id:4 diff:5

 F.

test1, id:1 diff:3

( 3.00 分 )

## 三、判断题 (共 10.00 分)





( 1.00 分 )

3.

方法drawRect需要四对说明画一个矩形所用的位置点参数

 A.

正确

 B.

错误

( 1.00 分 )



( 1.00 分 )

9.

##### 所有的字符输出流都从OutputStreamWriter类继承。

 A.

正确

 B.

错误

( 1.00 分 )

