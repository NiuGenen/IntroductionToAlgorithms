
cc = gcc

all: test_nt

test_nt: nt.c nt.h nt_test.c
	cc -o test_nt -g \
		nt.c nt.h nt_test.c

.PHONY: clean
clean:
	rm -rf *.o test_*
