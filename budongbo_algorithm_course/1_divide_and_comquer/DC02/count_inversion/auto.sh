#!/bin/bash 

make clean 
make 
echo "6 13 8 5 3 2 1" > asd
./test < asd
echo "1 1" > qwe
./test < qwe
echo "2 3 1" > zxc
./test < zxc
echo "2 4 1" > sdf
./test < sdf
echo "3 10 1 3" > wer
./test < wer
