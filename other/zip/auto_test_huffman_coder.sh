#!/bin/bash 

ENINPUT=./asd.bit
ENOUTPUT=./asd.bit.hfm
DEOUTPUT=./asd.bit.mfh

make clean 
make 
rm -rf ${ENINPUT} ${ENOUTPUT} ${DEOUTPUT}
echo "ahsjxkwehjfbvhgdodihwjebvrftruiiujwehdvjscyjhzngdqiuwgvfqhjegdasnbbd" > ${ENINPUT}
echo "========================= TEST ENCODE ========================="
./test_huffman_coder -e -i ${ENINPUT} -o ${ENOUTPUT} -t dynamic
echo "========================= TEST DECODE ========================="
./test_huffman_coder -d -i ${ENOUTPUT} -o ${DEOUTPUT}
echo "====== cat src file ======"
echo "Source file = ${ENINPUT}"
cat ${ENINPUT}
echo "====== cat dst file ======"
echo "Destination file = ${DEOUTPUT}"
cat ${DEOUTPUT}
