#!/bin/bash
cc -Wall tcpm.c

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
echo "./a.out ./temp/1.txt Aboslute path "
./a.out ./temp/1.txt /home/mli27/apue/hw2/temp/1.txt
echo \\n

echo "5.two different file"
echo "./a.out ./temp/1.txt ./temp/2.txt"
echo "This is 1st file" > ./temp/1.txt
echo "This is 2sd  file" > ./temp/2.txt
echo "1: "
cat ./temp/1.txt
echo "2: "
cat ./temp/2.txt
./a.out ./temp/1.txt ./temp/2.txt
echo "after"
echo "1. "
cat ./temp/1.txt
echo "2. "
cat ./temp/2.txt
echo \\n

echo "6. In Same Directory"
echo "./a.out ./temp/1.txt ./temp/"
./a.out ./temp/1.txt ./temp
echo \\n


echo "7.copy file to directory"
echo "./a.out ./temp/1.txt  ./temp/test"
./a.out ./temp/1.txt ./temp/test

ls -l ./temp/test
echo "cat the file"
cat ./temp/test/1.txt
rm ./temp/test/1.txt
echo \\n


echo "8.copy file to directory where the file exist but not the same"
echo "./a.out ./temp/2.txt ./temp/test"
echo "woca !!!" > ./temp/test/2.txt
echo "before:"
cat ./temp/test/2.txt
echo //n
./a.out ./temp/2.txt ./temp/test
echo "ls the file"
ls -l ./temp/test
echo "file content"
cat ./temp/test/2.txt
echo \\n










