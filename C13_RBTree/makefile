cc = gcc 

all:test_rb_tree

# test rb_tree 
 
_rb_tree = rb_tree.c rb_tree.h

test_rb_tree: rb_tree_test.c ${_rb_tree}
	cc -o test_rb_tree -g \
	   	rb_tree_test.c ${_rb_tree}

.PHONY: clean

clean:
	rm -rf test_rb_tree
