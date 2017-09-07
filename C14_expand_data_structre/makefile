all:test_order_statistic

order_statistic = order_statistic_tree.c order_statistic_tree.h

test_order_statistic: order_statistic_tree_test.c ${order_statistic}
	gcc -o test_order_statistic -g \
		order_statistic_tree_test.c ${order_statistic}

clean:
	rm -rf test_order_statistic
