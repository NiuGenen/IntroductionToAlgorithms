#!/bin/bash 

GENERATE_DATA_NUM=102400
DATA_FILE_NAME=interval_tree.random
RESULT_FILE_NAME=result_test_interval_tree

    rm -rf ${DATA_FILE_NAME} ${RESULT_FILE_NAME}
    make clean
    make
    ./test_interval_tree_data_generator ${GENERATE_DATA_NUM} ${DATA_FILE_NAME}
    ./test_interval_tree ${DATA_FILE_NAME} > ${RESULT_FILE_NAME}
    less result_test_interval_tree

