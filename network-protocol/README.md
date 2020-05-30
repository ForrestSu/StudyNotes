
# 协议报文抓包-清单

## TCP 接收到出错, 每隔 0.5s 重传握手请求
DATE: 2019-12-03 18:16:00  
方法： 使用telnet 连接一个已存在的IP，但是端口未开放  
文件：tcp/tcp_error_retransmission.pcapng  

## DHCP 协议抓包
DATE: 2019-11-28 15:12:00  
方法:禁用网卡，然后开启网卡，同时开启抓包程序即可。  
文件: dhcp/dhcp.pcapng  

可以看到 DHCP 向 255.255.255.255 这个地址发送DHCP 广播，目标端口为 67.

## Euraka Client 抓包
DATE: 2020-05-29 19:42:16  
文件: spring-euraka/euraka-client.pcapng 


## ChangeLog

- 2020-05-29 19:42:16 add [euraka-client.pcapng]
- 2019-12-03 18:16:00 add [tcp_error_retransmission.pcapng]
- 2019-11-28 15:12:00 add [dhcp.pcapng]
