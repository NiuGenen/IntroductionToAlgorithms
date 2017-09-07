all:test_dbg_info

cc = gcc 

_dbg_info = dbg_info.h 

test_dbg_info: dbg_info_test.c ${_dbg_info}
	cc -o test_dbg_info -g \
		dbg_info_test.c ${_dbg_info}

clean:
	rm -rf test_dbg_info
