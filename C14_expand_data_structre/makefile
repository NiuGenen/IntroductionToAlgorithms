cc = gcc 

all:dir test_order_statistic test_interval_tree

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

interval_tree = interval_tree.c interval_tree.h

test_interval_tree: interval_tree_test.c ${interval_tree} ${ask_time} ${dbg_info}
	cc -o test_interval_tree -g \
		interval_tree_test.c ${ask_time} ${dbg_info} ${interval_tree}

.PHONY: clean

clean:
	rm -rf test_order_statistic test_interval_tree
