#!/bin/bash

curdir=`pwd`
echo "append to ~/.zshrc: "
cat ${curdir}/zshrc >> ~/.zshrc

dest="/usr/local/bin"
echo "install <myunzip.py> into: "${dest}
ln -s ${curdir}/myunzip.py ${dest}/myunzip.py
