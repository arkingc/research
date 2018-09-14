#!/bin/bash

if [ ! -n "$1" ];then
    read -p "input number of containers:" NUM
else
    NUM=$1
fi

for i in $(seq 1 $NUM)
do
    ct_name=ct-$i
    docker stop $ct_name
    docker rm $ct_name
done
