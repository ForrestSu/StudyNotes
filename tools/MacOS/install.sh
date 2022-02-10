#!/bin/bash

echo 'alias ss="lsof -nP -i4TCP | grep LISTEN"' >> ~/.zshrc

dir=`pwd`
dst="/usr/local/bin"

echo "install <myunzip> into: "$dst
ln -s $dir/myunzip.py $dst/myunzip.py
