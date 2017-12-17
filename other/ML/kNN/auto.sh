#!/bin/bash 

cp ../../zip/bb.c ./
cp ../../zip/bb.h ./
make clean
make 
echo "----------------------------------------------------------"
./test_kNN
