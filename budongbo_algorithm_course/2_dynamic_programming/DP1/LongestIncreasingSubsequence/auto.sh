#!/bin/bash 

make clean 
make 
echo "2
4
1 2 3 1
7
1 7 3 5 9 4 8" > asd
./test < asd 
echo "---------------"
echo "3
7
1 9 10 5 11 2 13
2
2 -1
5
1 2 2 4 3" > zxc
./test < zxc
echo "---------------"
echo "1
1
1" > qwe
./test < qwe
echo "---------------"
echo "0" > wer
./test < wer
echo "---------------"
echo "1
0" > sdf
./test < sdf
echo "---------------"
echo "1
8
2 2 1 2 5 2 4 6" > xcv
./test < xcv
