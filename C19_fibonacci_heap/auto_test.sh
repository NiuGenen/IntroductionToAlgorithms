#!/bin/bash

MIN=1
MAX=1024
RANDOM_DATA=./fibonacci.random
RESULT=./result_fibonacci

rm -rf ${RANDOM_DATA} ${RESULT}

make clean
make 
./test_fibonacci_random ${MIN} ${MAX} ${RANDOM_DATA}
./test_fibonacci ${RANDOM_DATA} > ${RESULT}
less ${RESULT}
