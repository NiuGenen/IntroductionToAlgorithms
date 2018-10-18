#!/bin/bash 

echo "------automake.sh------"

if [ -f "./automake.sh" ]; then 
    ./automake.sh 
else 
    echo "[ERROR] No automake.sh to run"
fi 

echo "------autotest.sh------"

if [ -f "./autotest.sh" ]; then 
    ./autotest.sh 
else 
    echo "[ERROR] No autotest.sh to run"
fi

echo "------finish auto.sh------"
