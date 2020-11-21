#!/bin/bash

echo 'alias ss="lsof -nP -i4TCP | grep LISTEN"' >> ~/.zshrc

dir=`pwd`
dst="/usr/local/bin"

echo "install myunzip,get_ss_port into: "$dst
ln -s $dir/myunzip.py $dst/myunzip.py
#ln -s $dir/get_ss_port.sh $dst/get_ss_port.sh

