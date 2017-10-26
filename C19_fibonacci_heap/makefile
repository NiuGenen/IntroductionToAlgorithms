
cc = gcc

all: test_fibonacci test_fibonacci_random

fibonacci_heap := fibonacci_heap.c fibonacci_heap.h
CFLAGS := -g -lm

test_fibonacci: fibonacci_heap_test.c ${fibonacci_heap}
	cc -o test_fibonacci\
		fibonacci_heap_test.c ${fibonacci_heap} \
		${CFLAGS}

test_fibonacci_random: fibonacci_random.cc ${fibonacci_heap}
	cc -o test_fibonacci_random \
		fibonacci_random.cc ${fibonacci_heap} \
		${CFLAGS}

.PHONY: clean

clean:
	rm -rf test_*
