
cc = /usr/bin/gcc-5

all: kth_test kth_oj cp_oj cp_test cp_file cp_gen 

kth_test: kth_num_test.c kth_num.c kth_num.h
	cc -o kth_num -g kth_num.c kth_num.h kth_num_test.c

kth_oj: kth_num_oj.c kth_num.c kth_num.h
	cc -o kth_num_oj -g kth_num_oj.c kth_num.c kth_num.h

cp_oj: closest_pair.c closest_pair.h closest_pair_oj.c
	cc -o cp_oj -g closest_pair.c closest_pair.h closest_pair_oj.c -lm

cp_test: closest_pair.c closest_pair.h closest_pair_test.c
	cc -o cp_test -g closest_pair.c closest_pair.h closest_pair_test.c -lm

cp_file: closest_pair.c closest_pair.h closest_pair_file.c
	cc -o cp_file -g closest_pair.c closest_pair.h closest_pair_file.c -lm

cp_gen: point_generator.c
	cc -o cp_gen -g point_generator.c

clean:
	rm -rf kth_num kth_num_oj cp_oj cp_test cp_file cp_gen
