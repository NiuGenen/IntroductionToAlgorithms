
cc = /usr/bin/gcc-5 

#river_file = river.c river.h
river_file = river_submit.c
clean_file = test_river

all: river_test

river_test: ${river_file} #river_test.c
	cc -o test_river ${river_file} #river_test.c

clean:
	rm -rf ${clean_file}
