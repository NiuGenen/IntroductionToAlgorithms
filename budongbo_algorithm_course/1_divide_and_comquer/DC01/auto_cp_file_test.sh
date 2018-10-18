#!/bin/bash 

echo "--------------------"
rm asd
./cp_gen 10000 > /dev/null 
time ./cp_file
echo "--------------------"
rm asd
./cp_gen 20000 > /dev/null 
time ./cp_file
echo "--------------------"
rm asd
./cp_gen 40000 > /dev/null 
time ./cp_file
echo "--------------------"
rm asd
./cp_gen 60000 > /dev/null 
time ./cp_file
echo "--------------------"
rm asd
./cp_gen 80000 > /dev/null 
time ./cp_file
echo "--------------------"
