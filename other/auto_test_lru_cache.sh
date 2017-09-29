#!/bin/bash 

    make test_lru_cache
    ./test_lru_cache > result_test_lru_cache
    less result_test_lru_cache
