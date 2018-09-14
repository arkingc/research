#!/bin/bash

if [ ! -n "$1" ];then
    read -p "thread num:" THREAD_NUM
    read -p "test times:" TIMES
elif [ ! -n "$2" ];then
    THREAD_NUM=$1
    read -p "test times:" TIMES
else
    THREAD_NUM=$1
    TIMES=$2
fi

UPPER="upper"
LOWER="lower"
MERGED="merged"
WORK="work"

for i in $(seq 1 $TIMES)
do
    for j in $(seq 1 $THREAD_NUM)
    do
        mkdir $UPPER$j $MERGED$j $WORK$j
        mkdir $UPPER$j/dir1 $UPPER$j/dir2
        cp file1 $UPPER$j/dir1/
        sudo mount -t overlay overlay -olowerdir=$LOWER,upperdir=$UPPER$j,workdir=$WORK$j $MERGED$j
    done
    
    ./rename $THREAD_NUM
    
    for j in $(seq 1 $THREAD_NUM)
    do
        sudo umount $MERGED$j
        sudo rm -rf $UPPER$j $MERGED$j $WORK$j
    done
done
