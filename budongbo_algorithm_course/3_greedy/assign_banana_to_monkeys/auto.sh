#!/bin/bash 

if [ -f ./abm ]; then 
    rm -rf ./abm 
fi
gcc -o abm abm.c
