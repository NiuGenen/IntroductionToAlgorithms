
cc = gc 

all: test

test: test_huffman test_huffman_tree

test_huffman: huffman.c huffman.h huffman_test.c
	cc -o test_huffman -g \
		huffman_test.c huffman.c huffman.h

test_huffman_tree: huffman_tree.c huffman_tree.h huffman_tree_test.c
	cc -o test_huffman_tree -g \
		huffman_tree_test.c huffman_tree.c huffman_tree.h

.PHONY: clean

clean:
	rm -rf test_* result_* *.random
