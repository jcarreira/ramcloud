#!/bin/sh

# start coordinator in f16
echo "Launching coordinator in f16"
nohup ./obj.master/coordinator -C tcp:host=`hostname -s`,port=11100 -x zk:f16:2181 &

# start coordinator in f15
echo "Launching coordinator in f15"
ssh joao@f15 nohup repos/ramcloud/obj.master/coordinator -C tcp:host=f15,port=11100 -x zk:f16:2181 &

# start coordinator in f14
echo "Launching coordinator in f14"
ssh joao@f14 nohup repos/ramcloud/obj.master/coordinator -C tcp:host=f14,port=11100 -x zk:f16:2181 &
