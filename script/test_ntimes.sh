#!/bin/bash

if [ ! -n "$1" ];then
    read -p "test times:" TIMES
else
    TIMES=$1
fi

rm result

for i in $(seq 1 $TIMES)
do
    cache_clear.sh
    sleep 1
    pssh -O StrictHostKeyChecking=no -Ph iplist /mnt/a.out | grep "time" >> result
done

./data_analysis.sh $TIMES
