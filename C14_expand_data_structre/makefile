all:test_order_statistic test_interval_tree

order_statistic = order_statistic_tree.c order_statistic_tree.h

test_order_statistic: order_statistic_tree_test.c ${order_statistic}
	gcc -o test_order_statistic -g \
		order_statistic_tree_test.c ${order_statistic}

interval_tree = interval_tree.c interval_tree.h

test_interval_tree: interval_tree_test.c ${interval_tree}
	gcc -o test_interval_tree -g \
		interval_tree_test.c ${interval_tree}

clean:
	rm -rf test_order_statistic
	rm -rf test_interval_tree
