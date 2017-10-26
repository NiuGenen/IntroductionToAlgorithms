cc = gcc 

_test := test_time_asker test_dbg_info

all: ${_test} obj

_ask_time_obj := ask_time.o

_obj = ${_ask_time_obj}

# test_time_asker

test_time_asker: ask_time_test.c ${_ask_time_obj}
	cc -o test_time_asker -g \
		ask_time_test.c ${_ask_time_obj}

# test_dbg_info

test_dbg_info: dbg_info_test.c dbg_info.h
	cc -o test_dbg_info -g \
		dbg_info_test.c dbg_info.h

.PHONY: obj 

obj: ${_obj}

ask_time.o: ask_time.c ask_time.h
	cc -o ask_time.o -g -c ask_time.c

.PHONY: clean

clean:
	rm -rf ${_obj}
	rm -rf ${_test}
