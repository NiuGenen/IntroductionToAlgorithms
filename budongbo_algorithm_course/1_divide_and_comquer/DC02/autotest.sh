#!/bin/bash 

test_case="./_asd ./_qwe ./_zxc ./_wer ./_sdf"

if [ -f "./test_river" ]; then 
    let "num = num + 1"

    for file in ${test_case};
    do 
        if [ -f ${file} ]; then 
            echo "------test case ${file}------"
            cat ${file}
            echo "-----------------------------"
            ./test_river < ${file}
        fi
    done
fi
