make 

echo Begin running:
echo 1:./ls -isl main.c
./ls -isl main.c
echo sys
ls -isl main.c
echo 
echo 2:./ls . -islh main.c
./ls -islh main.c
echo sys
ls -islh main.c

echo 

echo 3:./ls -islk main.c
./ls -islk main.c
echo sys
ls -islk main.c

