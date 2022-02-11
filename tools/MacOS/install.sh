#!/bin/bash

# 覆盖zsh builtin 的 time 关键字
echo 'alias time="/usr/bin/time -h"' >> ~/.zshrc
echo 'alias ss="lsof -nP -i4TCP | grep LISTEN"' >> ~/.zshrc
echo 'alias sed=gsed' >> ~/.zshrc

dir=`pwd`
dst="/usr/local/bin"

echo "install <myunzip> into: "$dst
ln -s $dir/myunzip.py $dst/myunzip.py
