#!/bin/bash 

RESULT_FILE_NAME=result_test_order_statistic

    # clean 
    make clean 
    # compile 
    make 
    # test 
    ./test_order_statistic > ${RESULT_FILE_NAME}
    # view result 
    less ${RESULT_FILE_NAME}
