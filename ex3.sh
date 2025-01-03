#!/bin/bash
echo -n "Enter the number of terms: "
read n
if [[ ! "$n" =~ ^[0-9]+$ || "$n" -le 0 ]]; then
    echo "Please enter a valid positive integer."
    exit 1
fi
a=0
b=1
echo "Fibonacci Series up to $n terms:"
for ((i=1; i<=n; i++))
do
    echo -n "$a "
    fib=$((a + b))
    a=$b
    b=$fib
done
