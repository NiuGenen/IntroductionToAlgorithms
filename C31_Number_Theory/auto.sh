#!/bin/bash 

make clean
make 
echo ""
./test_nt -g
./test_nt -l
./test_nt -e
./test_nt -p > prime_test
cat prime_test | grep "1\ \-\ 0\ \-\ 0" > prime_test_1_0_0
