
# 协议报文抓包-清单

## TCP 接收到出错, 每隔 0.5s 重传握手请求
方法： 使用telnet 连接一个已存在的IP，但是端口未开放
文件：tcp_error_retransmission.pcapng

## DHCP 协议抓包
方法:禁用网卡，然后开启网卡，同时开启抓包程序即可。
文件: dhcp.pcapng

可以看到 DHCP 向 255.255.255.255 这个地址发送DHCP 广播，目标端口为 67.