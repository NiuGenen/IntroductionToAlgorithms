cc = gcc

all:test_hash test_hash_generator test_hash_table

_hash_func  = hash_func.c  hash_func.h
_hash_table = hash_table.c hash_table.h

# test_hash_table 

test_hash_table: hash_table_test.c ${_hash_table} ${_hash_func}
	cc -o test_hash_table -g \
		hash_table_test.c ${_hash_table} ${_hash_func}

# hash generator 

test_hash_generator: hash_str_generator.c
	cc -o test_hash_generator -g \
		hash_str_generator.c

# test hash 

test_hash: ${_hash_func} hash_test.c
	cc -o test_hash -g \
		hash_test.c ${_hash_func}

.PHONY: clean

clean:
	rm -rf test_hash test_hash_generator test_hash_table
	rm -rf *.random result_*
