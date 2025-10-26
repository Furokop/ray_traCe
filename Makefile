NAME = ray_trace
SRC = src

# Libraries to link, given by names
LIBS = m
# Variable generated from libs to pass to linker
LDLIBS := $(addprefix -l,$(LIBS))

DOCFILE_TMP = docstmp
DOCFILE = docs

CFLAGS += -Wall -MMD
# C compiler flags
ifeq ($(TEST), 1)
	CFLAGS += -DTEST=1
endif

# Debug symbols
ifeq ($(DEBUG), 1)
	# rdyanmic for getting pretty stack trace names
	CFLAGS += -g -O0 -DRT_DEBUG=1 -rdynamic
else
	CFLAGS += -O3 
endif

# Find files using the find program
files = $(shell find $(SRC) -name "*.c")
# .c files stripped from the suffix
# example.c to example
files_base = $(basename $(files))
# example to example.o
objects = $(addsuffix .o,$(files_base))

ray_trace: build

.PHONY: build
build: $(objects)
	$(CC) $(CFLAGS) -o $(NAME) $(objects) $(LDLIBS)

$(objects): %.o: %.c

.PHONY: clean
clean:
	rm ray_trace $(objects) $(addsuffix .d,$(files_base))

# Builds to a temporary directory
.PHONY: docbuild
docbuild:
	doxygen

# Renames the temporary doc directory to the ordinary one
.PHONY: docmove
docmove: docbuild
	mv $(DOCFILE_TMP) $(DOCFILE)/

.PHONY: docgen
docgen: docmove

# Use to regenerate when docs directory already exists
.PHONY: docregen
docregen: docdel docgen

.PHONY: docdel
docdel:
	rm -rf $(DOCFILE)

.PHONY: dochtmlopen
dochtmlopen:
	xdg-open $(DOCFILE)/html/index.html

lint: build clean

-include $(SRC)/*.d
