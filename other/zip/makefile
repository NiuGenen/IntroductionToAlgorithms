
cc = gc 

all: test

test: test_huffman test_huffman_tree test_bb test_huffman_coder

test_huffman: huffman.c huffman.h huffman_test.c
	cc -o test_huffman -g \
		huffman_test.c huffman.c huffman.h

test_huffman_tree: huffman_tree.c huffman_tree.h huffman_tree_test.c
	cc -o test_huffman_tree -g \
		huffman_tree_test.c huffman_tree.c huffman_tree.h

test_bb: bb.c bb.h bb_test.c
	cc -o test_bb -g \
		bb.c bb.h bb_test.c

test_huffman_coder: huffman_tree.c huffman_tree.h bb.c bb.h huffman_coder.c
	cc -o test_huffman_coder -g \
		huffman_tree.c huffman_tree.h bb.c bb.h huffman_coder.c

.PHONY: clean

clean:
	rm -rf test_* result_* *.random *.hfm *.mfh
