CC := g++
CFLAGS := -Wall -g

PROG_OUT := torrent.o
PROG_DIR := src
PROG_IMPL_FILES := main.cpp parse_arguments.cpp utils.cpp decode_command.cpp bencode.cpp
PROG_FULLNAME := $(PROG_DIR)/$(PROG_OUT)

TEST_OUT := test.o
TEST_DIR := tests
TEST_IMPL_FILES := test.cpp bencode_tests.cpp ../$(PROG_DIR)/bencode.cpp
TEST_FULLNAME := $(TEST_DIR)/$(TEST_OUT)

define COMPILE =
cd $(1);\
$(CC) $(2) -o $(3) $(CFLAGS);
endef

all: clean

test:
	$(call COMPILE,$(TEST_DIR),$(TEST_IMPL_FILES),$(TEST_OUT))
	./$(TEST_FULLNAME)

compile:
	$(call COMPILE,$(PROG_DIR),$(PROG_IMPL_FILES),$(PROG_OUT))

clean:
	rm -rf $(TEST_DIR)/$(TEST_OUT) $(PROG_DIR)/$(PROG_OUT)