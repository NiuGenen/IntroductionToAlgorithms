
cc = gcc

time_asker:= ../util/ask_time.o

all: test_fibonacci test_fibonacci_random
	${MAKE} -C ../util obj

fibonacci_heap := fibonacci_heap.c fibonacci_heap.h
CFLAGS := -g -lm

test_fibonacci: fibonacci_heap_test.c ${fibonacci_heap} ${time_asker}
	cc -o test_fibonacci\
		fibonacci_heap_test.c ${fibonacci_heap} ${time_asker}\
		${CFLAGS}

test_fibonacci_random: fibonacci_random.cc ${fibonacci_heap}
	cc -o test_fibonacci_random \
		fibonacci_random.cc ${fibonacci_heap} \
		${CFLAGS}

.PHONY: clean

clean:
	rm -rf test_*
	${MAKE} -C ../util clean
