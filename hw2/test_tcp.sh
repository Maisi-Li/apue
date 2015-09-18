#!/bin/bash
cc -Wall tcp.c

#1. Three arguement
echo "1.three arguemnts"
echo "./a.out dd dd ddd"
./a.out dd dd ddd
echo \\n

echo "2.Test if the first argument is not a file"
echo "tcp dd dd"
./a.out dd ddd
echo \\n

echo "3.Test if the second argument is not a file or directory"
echo "tcp ./temp/1.txt ddd"
./a.out ./temp/1.txt ddd
echo \\n

echo "4.two arguments are the same file"
echo "./a.out ./temp/1.txt ./temp/1.txt"
./a.out ./temp/1.txt ./temp/1.txt
echo \\n
