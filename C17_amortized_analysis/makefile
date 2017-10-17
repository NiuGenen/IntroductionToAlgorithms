
cc = gcc

all: test_dynamic_table

_dynamic_table := dynamic_table.c dynamic_table.h

test_dynamic_table: dynamic_table_test.c ${_dynamic_table}
	cc -o test_dynamic_table -g \
		dynamic_table_test.c ${_dynamic_table}

.PHONY: clean

clean:
	rm -rf test_dynamic_table result_* *.random
