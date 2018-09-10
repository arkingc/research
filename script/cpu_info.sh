#!/bin/bash

cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l

cat /proc/cpuinfo| grep "cpu cores"| uniq

cat /proc/cpuinfo| grep "processor"| wc -l
