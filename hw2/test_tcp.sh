#!/bin/bash
cc -Wall tcp.c

#1. Three arguement
echo "1.three arguemnts"
./a.out ddd aaa ddd

echo "1.Test if the first argument is not a file"
./a.out dd ddd
