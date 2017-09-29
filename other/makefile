cc = gcc

_test := test_hash \
	test_hash_generator test_hash_table \
	test_bloom_filter \
	test_linked_list \
	test_lru_cache

all: ${_test}

_obj := hash_func.o hash_table.o bloom_filter.o linked_list.o lru_cache.o

# test_linked_list 

test_linked_list: linked_list_test.c linked_list.o
	cc -o test_linked_list -g linked_list_test.c linked_list.o

# test_bloom_filter

test_bloom_filter: bloom_filter_test.c hash_func.o bloom_filter.o
	cc -o test_bloom_filter -g bloom_filter_test.c hash_func.o bloom_filter.o

# test_hash_table 

test_hash_table: hash_table_test.c hash_func.o hash_table.o
	cc -o test_hash_table -g hash_table_test.c hash_func.o hash_table.o

# hash generator 

test_hash_generator: hash_str_generator.c
	cc -o test_hash_generator -g hash_str_generator.c

test_lru_cache: lru_cache_test.c lru_cache.o
	cc -o test_lru_cache -g lru_cache_test.c lru_cache.o

# test hash 

test_hash: hash_test.c hash_func.o
	cc -o test_hash -g hash_test.c hash_func.o

.PHONY: obj 

obj: ${_obj}

hash_func.o: hash_func.c hash_func.h
	cc -o hash_func.o -c hash_func.c -g

hash_table.o: hash_table.c hash_table.h
	cc -o hash_table.o -c hash_table.c -g

bloom_filter.o: bloom_filter.c bloom_filter.h
	cc -o bloom_filter.o -c bloom_filter.c -g

linked_list.o: linked_list.c linked_list.h
	cc -o linked_list.o -c linked_list.c -g

lru_cache.o: lru_cache.h lru_cache.c
	cc -o lru_cache.o -c lru_cache.c -g

.PHONY: clean

clean:
	rm -rf ${_test}
	rm -rf *.o
	rm -rf *.random 
	rm -rf result_*
