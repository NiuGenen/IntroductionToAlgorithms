#!/bin/bash 

make clean 
make 
echo "3 3
2
3 2 5
3 2 2
4 2 1" > asd 
./test < asd
echo "4 3
3
7 1 5
4 6 2
4 2 1
5 7 3" > zxc
./test < zxc
