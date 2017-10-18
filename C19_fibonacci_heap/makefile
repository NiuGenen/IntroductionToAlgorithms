
cc = gcc

all: test_fibonacci_heap

fibonacci_heap := fibonacci_heap.c fibonacci_heap.h
CFLAGS := -g -lm

test_fibonacci_heap: fibonacci_heap_test.c ${fibonacci_heap}
	cc -o test_fibonacci_heap \
		fibonacci_heap_test.c ${fibonacci_heap} \
		${CFLAGS}

.PHONY: clean

clean:
	rm -rf test_fibonacci_heap result_* *.random
