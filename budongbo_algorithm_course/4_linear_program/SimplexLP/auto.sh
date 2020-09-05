#!/bin/bash 

if [ -f ./tst ]
then
   rm ./tst
fi 

gcc-8 -o tst s.volunteer.c
./tst
