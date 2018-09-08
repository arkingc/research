#!/bin/bash

if [ ! -n "$1" ];then
    read -p "input number of containers:" NUM
else
    NUM=$1
fi

MOUNT_PATH=`pwd`/dockerMount

rm iplist

for i in $(seq 1 $NUM)
do
    ct_name=ct-$i
    
    docker run -td --name $ct_name -v $MOUNT_PATH:/mnt ubuntu:cow512M /bin/bash
    
    docker exec $ct_name /etc/init.d/ssh start
    
    declare -i ipidx=$i+1
    echo root@172.17.0.$ipidx >> iplist
done

pssh -O StrictHostKeyChecking=no -Ph iplist /mnt/a.out
