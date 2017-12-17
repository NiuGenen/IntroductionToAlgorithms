
cc = gcc

all:test_kNN

test_kNN: kNN.c kNN.h kNN_test.c bb.c bb.h
	cc -o test_kNN -g \
		kNN.c kNN.h kNN_test.c bb.c bb.h

.PHONY: clean
clean:
	rm -rf test_*
