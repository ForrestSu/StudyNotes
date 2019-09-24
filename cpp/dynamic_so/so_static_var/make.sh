#!/bin/bash

g++ --shared -fPIC -g -Wall a.cpp -o liba.so

g++ --shared -fPIC -g -Wall b.cpp -o libb.so

# The -x option starts a BASH shell in debug mode
set -x
# export LD_LIBRARY_PATH=$PWD
g++ main.cpp -L. -la -lb -o ab_main
./ab_main

g++ main.cpp -L. -lb -la -o ba_main
./ba_main

# static 变量保存可执行文件或 so 的数据段（data segement) 中，并且有初始值;
# 可见 static 只认第一次初始化