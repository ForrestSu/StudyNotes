#/usr/bin/bash
result=`ls core.*`
icount=0;

for var in $result
do
  echo $var
  timeout -s9  0.1 gdb $var 2>/dev/null |grep "generated"
  let icount++
done

echo "total cat uvframe cores: "$icount
exit
