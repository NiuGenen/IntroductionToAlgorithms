cc = gcc 

all:test_rb_tree

# test rb_tree 
 
_rb_tree = rb_tree.o

test_rb_tree: rb_tree_test.c ${_rb_tree}
	cc -o test_rb_tree -g \
	   	rb_tree_test.c ${_rb_tree}

rb_tree.o: rb_tree.c rb_tree.h
	cc -o rb_tree.o -c rb_tree.c -g

.PHONY: clean

clean:
	rm -rf rb_tree.o
	rm -rf test_rb_tree
