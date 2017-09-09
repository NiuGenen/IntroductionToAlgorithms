cc = gcc 

.PHONY: test_time_asker

_time_asker = ask_time.h ask_time.c

test_time_asker: ask_time_test.c ${_time_asker}
	gcc -o test_time_asker -g \
		ask_time_test.c ${_time_asker}

.PHONY: test_dbg

_dbg_info := dbg_info.h 

test_dbg:test_dbg_info

test_dbg_info: dbg_info_test.c ${_dbg_info}
	cc -o test_dbg_info -g \
		dbg_info_test.c ${_dbg_info}

.PHONY: subdirs $(SUBDIRS)

SUBDIRS := $(shell find . -maxdepth 1 -type d )
SUBDIRS := $(basename $(patsubst ./%,%,$(SUBDIRS)))

subdirs: $(SUBDIRS)
	#@echo ${SUBDIRS}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

.PHONY: clean

clean_dirs := $(addprefix _clean_, $(SUBDIRS) )

clean: $(clean_dirs)
	rm -rf test_dbg_info

$(clean_dirs):
	$(MAKE) -C $(patsubst _clean_%,%,$@) clean

