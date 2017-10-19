
cc = gc 

all: test

test: test_huffman

test_huffman: huffman.c huffman.h huffman_test.c
	cc -o test_huffman -g \
		huffman_test.c huffman.c huffman.h

.PHONY: clean

clean:
	rm -rf test_* result_* *.random
