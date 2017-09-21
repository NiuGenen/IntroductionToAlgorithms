cc = gcc 

all: test_time_asker test_dbg_info

# test_time_asker

_time_asker = ask_time.h ask_time.c

test_time_asker: ask_time_test.c ${_time_asker}
	cc -o test_time_asker -g \
		ask_time_test.c ${_time_asker}

# test_dbg_info

_dbg_info := dbg_info.h 

test_dbg_info: dbg_info_test.c ${_dbg_info}
	cc -o test_dbg_info -g \
		dbg_info_test.c ${_dbg_info}

.PHONY: clean

clean:
	rm -rf test_dbg_info
	rm -rf test_time_asker
