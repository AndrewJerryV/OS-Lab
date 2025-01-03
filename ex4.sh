#!/bin/bash
echo -n "Enter the file name: "
read file_name
if [ ! -f "$file_name" ]; then
    echo "Error: File '$file_name' not found!"
    exit 1
fi
echo "Enter the word to search:"
read search_word
occurrences=$(grep -o -i -w "$search_word" "$file_name" | wc -l)
echo "'$search_word' occurred $occurrences times in the file '$file_name'."
