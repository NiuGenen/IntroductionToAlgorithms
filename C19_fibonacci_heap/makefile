
cc = gcc

all: test_fibonacci_heap

fibonacci_heap := fibonacci_heap.c fibonacci_heap.h

test_fibonacci_heap: fibonacci_heap_test.c ${fibonacci_heap}
	cc -o test_fibonacci_heap -g \
		fibonacci_heap_test.c ${fibonacci_heap}

.PHONY: clean

clean:
	rm -rf test_fibonacci_heap result_* *.random
