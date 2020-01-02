# openpgm test

## 准备工作

需要两个依赖包: `openpgm` `glib2`

Archlinux 环境下：  
>pacman -S  libpgm glib2

Centos7 环境下：   
> yum install -y glib2-devel opengpm-devel

## 编译 & 运行 

> sh build.sh

> ./pgm_recv  -n 239.0.0.10 -p 5555

> ./pgm_send  -n 239.0.0.10 -p 5555 -r 1000000 -l 

同时使用 iperf 查看实时网速  
> ipref -B -n 


