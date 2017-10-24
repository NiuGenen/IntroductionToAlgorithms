#!/bin/bash 

ENINPUT=./asd.bit
ENOUTPUT=./asd.bit.hfm
#BBOUTPUT=~/asd.bit.hfm_
DEOUTPUT=./asd.bit.mfh

make clean 
make 
rm -rf ${ENINPUT} ${ENOUTPUT} ${DEOUTPUT}
echo "ahsjgdqiuwgvfqhjegdasnbbd" > ${ENINPUT}
echo "========================= TEST ENCODE ========================="
./test_huffman_encoder ${ENINPUT} ${ENOUTPUT}
#./test_bb ${ENOUTPUT} ${BBOUTPUT}
echo "========================= TEST DECODE ========================="
./test_huffman_decoder ${ENOUTPUT} ${DEOUTPUT}
echo "====== cat src file ======"
echo "Source file = ${ENINPUT}"
cat ${ENINPUT}
echo "====== cat dst file ======"
echo "Destination file = ${DEOUTPUT}"
cat ${DEOUTPUT}
