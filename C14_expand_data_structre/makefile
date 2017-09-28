cc = gcc 

_test := test_order_statistic test_interval_tree

all:dir ${_test}

ask_time := util/ask_time.h util/ask_time.c

dbg_info := util/dbg_info.h 

# dir 

.PHONY: dir 

dir_cwd    := $(shell pwd)
dir_cwd_p  := $(shell dirname ${dir_cwd})
dir_util   := ${dir_cwd_p}/

ask_time := $(foreach var,${ask_time},${dir_util}${var})
dbg_info := $(foreach var,${dbg_info},${dir_util}${var})

dir:
	echo ${dir_util}

# order_statistic

order_statistic = order_statistic_tree.c order_statistic_tree.h

test_order_statistic: order_statistic_tree_test.c ${order_statistic}
	cc -o test_order_statistic -g \
		order_statistic_tree_test.c ${order_statistic}

# interval_tree

interval_tree := interval_tree.c interval_tree.h
interval_tree_test_data_generator := interval_tree_test_data_generator.c

test_interval_tree: interval_tree_test.c ${interval_tree} ${ask_time} ${dbg_info} test_interval_tree_data_generator
	cc -o test_interval_tree -g \
		interval_tree_test.c ${ask_time} ${dbg_info} ${interval_tree}

test_interval_tree_data_generator: ${interval_tree_test_data_generator}
	cc -o test_interval_tree_data_generator -g \
		${interval_tree_test_data_generator}

.PHONY: clean

clean:
	rm -rf ${_test}
	rm -rf result_*
	rm -rf *.random
	rm -rf *_data_generator
