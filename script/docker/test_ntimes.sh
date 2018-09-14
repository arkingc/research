#!/bin/bash

if [ ! -n "$1" ];then
    read -p "test times:" TIMES
    read -p "prog name:" PROG
elif [ ! -n "$2" ];then
    TIMES=$1
    read -p "prog name:" PROG
else
    TIMES=$1
    PROG=$2
fi

rm result

for i in $(seq 1 $TIMES)
do
    cache_clear.sh
    sleep 1
    pssh -O StrictHostKeyChecking=no -Ph iplist /mnt/$PROG | grep "time" >> result
done

./data_analysis.sh $TIMES
