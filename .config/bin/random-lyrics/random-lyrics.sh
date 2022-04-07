#!/bin/bash

time=$(($2/1000))

while read y
do 
    echo $y
    sleep $time
done < $1
