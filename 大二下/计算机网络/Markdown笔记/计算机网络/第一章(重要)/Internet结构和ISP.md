# 互联网络结构：网络的网络
端系统通过接入ISPs接入互联网
因此，接入ISPs之间必须互联
从**结构演变的历程**梳理不同的网络结构：
## 1.网状结构：ISP两两互联
花费巨大，可拓展性scalebility差
![[网络结构1.png]]

## 2. 全局ISP和接入ISP
![[网络结构2.png]]

## 3.合作竞争
**有利可图**：上层Global ISP互联为下层ISP提供付费服务
**竞争：** 多个上层ISP为下层提供不同服务
**合作：** 同等的上层ISP互联->通过**IXP(Internet Exchange Point)形成对等Peer关系** 
![[网络结构3.png]]
## 4.等级化结构继续细分
从上到下根据区域不断细分，增加Regional ISP
![[网络结构4.png]]

## 5.当今互联网
![[网络结构5.png]]
ISP：提供网络，但费用高服务不够
ICP：提供业务
增加**内容提供商网络(content provider network)**
谷歌，微信.... 也与第一层ISP连接
因此在全球范围搭建了很多Data Centre 用专网互联，且离ISP很近
**既减少ISP成本，又能向客户提供更好服务**

## Nerwork of networks
![[Pasted image 20250321190959.png]]
**一个分组需要经过很多ISP**
![[分组经过ISP.png]]
**POP:** 高层ISP**面向客户**网络的接入点，涉及费用结算 
- 如一个低层ISP接入多个高层ISP，多宿（multi home）
**IXP：** 多个**对等ISP**互联互通之处，通常不涉及费用结算 
- 对等接入