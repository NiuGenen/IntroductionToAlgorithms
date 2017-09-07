all:test_rb_tree

_rb_tree = rb_tree.c rb_tree.h

test_rb_tree: rb_tree_test.c ${_rb_tree}
	gcc -o test_rb_tree -g \
	   	rb_tree_test.c ${_rb_tree}

clean:
	rm -rf test_rb_tree
