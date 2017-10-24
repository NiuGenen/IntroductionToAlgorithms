
cc = gc 

all: test

test: test_huffman test_huffman_tree test_bb test_huffman_encoder test_huffman_decoder

test_huffman: huffman.c huffman.h huffman_test.c
	cc -o test_huffman -g \
		huffman_test.c huffman.c huffman.h

test_huffman_tree: huffman_tree.c huffman_tree.h huffman_tree_test.c
	cc -o test_huffman_tree -g \
		huffman_tree_test.c huffman_tree.c huffman_tree.h

test_bb: bb.c bb.h bb_test.c
	cc -o test_bb -g \
		bb.c bb.h bb_test.c

test_huffman_encoder: huffman_tree.c huffman_tree.h bb.c bb.h huffman_encoder.c
	cc -o test_huffman_encoder -g \
		huffman_tree.c huffman_tree.h bb.c bb.h huffman_encoder.c

test_huffman_decoder: huffman_tree.c huffman_tree.h bb.c bb.h huffman_decoder.c
	cc -o test_huffman_decoder -g \
		huffman_tree.c huffman_tree.h bb.c bb.h huffman_decoder.c

.PHONY: clean

clean:
	rm -rf test_* result_* *.random *.hfm *.mfh
