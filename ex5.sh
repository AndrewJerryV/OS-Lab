#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: $0 <start_number> <end_number>"
    exit 1
fi

start=$1
end=$2

if ! [[ $start =~ ^[0-9]+$ && $end =~ ^[0-9]+$ ]]; then
    echo "Error: Both arguments must be positive integers."
    exit 1
fi

is_palindrome() {
    num=$1
    reversed=$(echo $num | rev)
    if [ $num -eq $reversed ]; then
        return 0  
    else
        return 1  
    fi
}
echo "Palindrome numbers between $start and $end:"
for ((i=start; i<=end; i++)); 
do
    if is_palindrome $i; then
        echo $i
    fi
done
