#!/bin/bash 

make clean 
make 
echo "[$ cat ~/asd.bit ]"
cat ~/asd.bit
echo "[$ cat ~/qwe.bit ]"
cat ~/qwe.bit
echo "[$ rm -rf ~/qwe.bit]"
rm -rf ~/qwe.bit
echo "[$ ./test_bb ~/asd.bit ~/qwe.bit ]"
./test_bb ~/asd.bit ~/qwe.bit
echo "[$ cat ~/qwe.bit ]"
cat ~/qwe.bit
