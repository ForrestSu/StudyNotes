#!/bin/bash
##### The first parameter as the working path #####
predir=
if [ -n "$1" ]; then
    echo "will cd into dir: $1"
    predir=`pwd`
    cd  "$1"
fi

##### work #####
result=`ls *.proto`
suffix=".proto"
output_dir="./gen_cpp"
cpp_suffix=".pb.cc"
cpp_suffix_replace=".pb.cpp"

if [ ! -d "$output_dir" ];then
    mkdir $output_dir
fi
for var in $result
do
    echo $var
    protoc --cpp_out=$output_dir --proto_path=.  $var
    mv $output_dir/${var%$suffix}$cpp_suffix  $output_dir/${var%$suffix}$cpp_suffix_replace
done

##### done #####
if [ -n "$predir" ]; then
  echo "cd back: $predir"
  cd $predir
fi
 
 
 




