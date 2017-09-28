cc = gcc

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

$(clean_dirs):
	$(MAKE) -C $(patsubst _clean_%,%,$@) clean
