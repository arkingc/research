#!/bin/bash

if [ ! -n "$1" ];then
    read -p "test times:" TIMES
else
    TIMES=$1
fi

sort result | awk -v times=$TIMES 'BEGIN{count = 0;sum=0} {sum=sum+$3;count=count+1} count==times{count=0;print sum/times;sum=0}'

