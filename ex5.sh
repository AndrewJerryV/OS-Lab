#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two positive numbers are required"
    exit 1
fi

start=$1
end=$2

echo "Palindrome numbers between $start and $end:"
for ((i=start; i<=end; i++)); 
do
    num=$i
    original=$num
    reversed=0
    while [ $num -gt 0 ]; do
        remainder=$((num % 10))
        reversed=$((reversed * 10 + remainder))
        num=$((num / 10))
    done
    if [ $original -eq $reversed ]; then
        echo $i
    fi
done
