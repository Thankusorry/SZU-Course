**Transmission Control Protocol 传输控制协议**
## 特点
1. ==**可靠传输服务**==：不丢失、不重复、有序
	<u>可靠是对端对端来说的，不是传输过程中的完全可靠</u>
2. ==**面向连接**==
3. 支持==**字节流**==**传输**
4. 支持==**全双工**==服务
5. 支持建立多个==**并发TCP连接**==（服务器同时响应多个连接）
6. ==**不支持广播**==
### **全双工**
通信方式：
1. 单工：单方向通信
2. 半双工：双向异步
3. **全双工： 双向同步，既能接受，又能发送**
	更多体现在三次握手四次挥手

## ！！！TCP报文格式
TCP有如此优势，得益于其**报头设计之复杂**
![[TCP报文格式.png|800]]

1. **源端口号，目的端口号**：和UDP相同，32bit 4字节

### **2.发送序号 4字节**
发送序号（**==seq==**）：**表示TCP数据字段的第一个字节的序号**
<font color="#c0504d">“我的数据从哪里开始”</font>
**连接建立时（即SYN=1），==初始序号（ISN）由随机数生成器生成==，发送端和接收端独立产生，可能不一样——Safety**
### **3.确实序号 4字节**
**确认序号（ack）**：**==只有当ACK位=1时有效==，表示发送此报文段的进程期望接收的下一个新字节的序号**
<font color="#c0504d">"你给的我收到了，我希望你下次新的数据从这个确认序号开始"</font>
确认序号=N+1，表示接收方已经成功接收了序号为N及之前的所有字节，要求发送方接下来应该发送起始序号为N+1的字节段
#### **例子**
**A->B** : seq=100 **ack=200** L =100  A数据范围是 100-199
B->A : **seq=200**  B已经成功接受了100-199的数据，从第200开始 **ack=200** 我希望你从200开始**L=150**
A->B :  **seq=200** ,ack =350，L=xxx数据没规律

**==特殊情况：请求连接和确认连接 都会消耗一个序号，ack必须+1==**
**==一般情况==：seq=接收报文的ack，ack=接收报文的seq+len**
### **4 报头长度 4bit**
**表示TCP报文首部的大小，==单位是4字节，取值范围是[5,15]==**
- **固定部分20B,填充部分[0,40]B**
- **报头长度：5** 对应 5×4=**20字节**，**首部长度为20字节，则选项填充为0**
- **报头长度：15** 对应 15×4=**60字节**，**首部长度为20字节，则选项填充为40B**

**和UDP区别：**
1. **==不包括数据部分，UDP总长度包含数据==** 
2. **TCP单位是4 Byte**

### **5 保留 6bit**
==**预留位，全0**==
### **6.控制字段：六个标志位**
**每个1bit**
**==重点：SYN，ACK，FIN==**
1. URG
2. **ACK**: **确认号字段的值有效**。**==只有当ACK＝1时，确认序号字段才有意义==**
3. PSH
4. RST
5. **SYN**: **==SYN=1,ACK=0 建立连接请求报文==**,如果确认建立连接，返回SYN=1,ACK=1
	- 代表**Connection Request**和**Connection Accepted**，用ACK位来区分这两种情况
6. **FIN**：**终止连接**，**==当FIN＝1时，表明数据已经发送完毕，并请求释放连接==**
### **7 窗口值 2B**
**窗口值**：指示当前进程**可以接收的数据长度 (单位: 字节)**
准备接收下一个TCP报文的**接收方**，**通知即将发送报文的发送方**下一个**报文中最多可以发送的字节数**      
**==发送方确定发送窗口的依据，是动态可变的==**

**数据保存在有限的缓冲区，目的是==流量控制==**
<u>发送窗口，拥塞窗口，接收窗口(通知窗口)</u>

### **8. 校验和 2B**
和UDP校验和异同点：
1. **计算方式相同**(都需要伪首部)
2. **TCP必选检验和，UDP可选**
3. **==伪首部协议号字段：UDP 17 | TCP 6==**

### 9.紧急指针 2B
### 10. 选项填充
**长度为0-40B（字节）**

### **MSS**
**TCP报文==数据部分的最大长度==，不包括TCP报头长度。==默认值为536字节==**

- **窗口长度**是收到的**数据累计和** 最大限制
- **MSS**是**单个TCP报文数据**最大长度、

# TCP基本通信过程
![[Pasted image 20250514230510.png|550]]
## 三次握手
![[三次握手示意图.png]]
### **第一次握手**
**发起方向接收方发送连接请求报文——[SYN=1，ACK=0]，Seq=x**
SYN=1的报文段不携带数据，但要消耗掉一个序号

### **第二次握手**
**接受方收到请求并同意建立连接，向请求方发送确认报文——[SYN=1,ACK=1],Seq=y,==ack=x+1==**

### **第三次握手**
**客户端收到确认，并对此进行确认——==ACK=1==,==Seq=x+1,ack=y+1==**


## 四次挥手

![[四次挥手示意图.png|600]]
**连接的关闭可以由任意一方主动发起**
### **发起方第一次挥手**
**发起方请求释放连接—[FIN=1],seq=u**
	**进入FIN-WAIT-1（终止等待1）状态**

### **接收方第二次挥手**
**接收方确认释放——[ACK=1],seq=v,==ack=u+1==**
	**进入了CLOSE-WAIT 关闭等待状态**
**==接收方依然可以进行数据传输==**

### **第三次挥手**
**发起方收到确认释放报文，等待服务器发送释放报文**
	**进入FIN-WAIT2(终止等待2)状态**

**接收方发完最后的数据，发送自己的释放报文——FIN=1,seq=w, ack=u+1**
	**进入LAST-ACK（最后确认）状态，等待最后数据确认**

### **第四次握手**
**发起方发送确认报文——ACK=1,ack=w+1,seq=u+1**
	**进入TIME-WAIT(时间等待)状态**
**经过==2MSL==才CLOSED**
**MSL：Maximum Segment Lifetime**
**==ACK报文可能丢失,2MSL确保丢弃可以重发==**

<font color="#ff0000">时间等待定时器</font>

成功握手后——保持计时器
## 易错点：
1. **发起方单方向释放连接——接收方仍然可以传输数据**
2. **第二次挥手和第三次挥手都是接收方，两次的含义完全不同**
- 第二次挥手是确认报文,给出自己的seq和 **==ack=seq+1==**
- 第三次挥手是传输完数据取消连接——**发送数据导致seq发生变化**
  **==但是收到数据不变，ack不变==**

**==非常重要的问题： 为什么是2MSL==**
1. **足够覆盖被动关闭方的多次 FIN 重传，保证网络中的所有报文都能够被丢弃，确保最后的 ACK 报文能够到达**
2. **防止旧连接的混淆：所有旧连接的报文都会被丢弃，确保新连接不会收到旧数据。**

  
**采用TCP协议的报文段在传输过程中不会丢失（×）**

### 确认序号错题
特殊阶段：建立连接和释放连接都会消耗一个确认序号

e.g.若主机甲与主机乙建立TCP连接时，发送的SYN段中的序号为1000，在断开连接时，主机甲发送给主机乙的FIN段中的序号是5001，则在无任何重传的情况下，甲向乙已经发送的应用层数据的字节数是（ **4000** ）

**实际数据部分： 1001~5000 共4000B**