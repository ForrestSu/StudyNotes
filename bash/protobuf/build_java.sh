#!/bin/bash

output_dir="./gen_java"
result=`ls *.proto`

if [ ! -d "$output_dir" ];then
    mkdir $output_dir
fi

for var in $result
do
    echo $var
    protoc --java_out=$output_dir --proto_path=.  $var
done





