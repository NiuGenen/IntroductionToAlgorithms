cc = gcc

all:test_hash

_hash_func = hash_func.c hash_func.h

test_hash: ${_hash_func} hash_test.c
	cc -o test_hash -g \
		hash_test.c ${_hash_func}

.PHONY: clean

clean:
	rm -rf test_hash
