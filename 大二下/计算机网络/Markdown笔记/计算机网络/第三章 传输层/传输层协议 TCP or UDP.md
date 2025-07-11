**根据网络应用的实际需求，决定选择传输协议：TCP、UDP**
### **1.采用网络设施的面向连接**
**<font color="#ff0000">握手：数据传输之前做好准备</font>**  
**仅端系统之间维持**
>[!fail]+ 和有连接的区别？
> 有连接：中间所有节点都知道
#### **TCP**
**Transmission Control Protocol 传输控制协议**
**原原本本** *不重复，不失序，不出错，不乱序* 
==**优点：**==
通过**TCP本身一系列的机制**保证可靠性
- **可靠、顺序传送数据**
- **流量控制**
- **拥塞控制**
==**应用：**== HTTP(Web)  FTP(文件传送)  Telnet（远程登录） SMTP(email)  
**==缺点：==** 不能保证 时间、吞吐量最小和安全
### **2.采用基础设施的无连接服务**
上来就发送，对方直接回应
#### **UDP**
User Datagram Protocol 用户数据报协议
- 无连接
- 不可靠
- 无流量控制——适合网络多媒体，服务多快，客户多块
- 无拥塞控制
==**优点：快！**==
1. **能否区分不同进程，而IP服务不能**
2. **无需建立连接：** 节约时间，更适合事务性的应用
3. **不做保证可靠性的工作：** 适合对实时性要求比较高而对正确性要求不高
4. **没有拥塞控制和流量控制**：**应用能够按照设定的速度传输数据**
==**应用：**==流媒体、远程会议、DNS、Internet电话
![[网络应用对传输层协议的选择.png]]

## 安全TCP
**TCP和UDP都不提供任何的安全性**
**SSL：加强版的TCP，提供加密**
HTTP<u>S</u>