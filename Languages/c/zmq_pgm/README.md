# zeromq pgm 协议测试

测试  zeromq pgm 的收发包效率

## 准备工作

需要两个依赖包: `openpgm` `libzmq`

Archlinux 环境下：  
>pacman -S  libpgm libzmq

Arch上面默认安装的zmq 是最新版 zeromq 4.3.2, ldd查看已经链接了pgm.so

Centos7 环境下：
> yum install -y opengpm-devel

Centos7上需要通过源码编译安装libzmq， 最新的release版本为 v4.3.2

(1) 先clone libzmq

(2) 采用cmake 编译即可

> mkdir build && cd build  
> cmake -DWITH_OPENPGM=ON -DCMAKE_INSTALL_PREFIX=/usr ..  
> make -j5  
> make install

提示：编译时还可以关闭 test,以及static lib，加快编译速度

> -DBUILD_TESTS=OFF  -DBUILD_STATIC=OFF

(3) 更新so cache,并采用ldd 验证一下

> ldconfig


## 编译 & 运行 

> make 

> ./sub_zmq "epgm://eth0;239.0.1.1:5555" 1000

> ./pub_zmq "epgm://eth0;239.0.1.1:5555" 1000 100001

同时使用 iperf 查看实时网速  
> ipref -B -n 

## Author

Sun Quan