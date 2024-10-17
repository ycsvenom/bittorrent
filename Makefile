CC := g++
CFLAGS := -Wall -g

PROG_OUT := torrent.o
PROG_DIR := src
PROG_IMPL_FILES := $(addprefix $(PROG_DIR)/,main.cpp parse_arguments.cpp utils.cpp decode_command.cpp display_info_command.cpp bencode.cpp)
PROG_FULLNAME := $(PROG_DIR)/$(PROG_OUT)

TEST_OUT := test.o
TEST_DIR := tests
TEST_IMPL_FILES := $(addprefix $(TEST_DIR)/,test.cpp bencode_tests.cpp) $(PROG_DIR)/bencode.cpp
TEST_FULLNAME := $(TEST_DIR)/$(TEST_OUT)

define COMPILE =
$(CC) $(1) -o $(2) $(CFLAGS);
endef

all: clean

debug:
	echo $(PROG_IMPL_FILES)

test:
	$(call COMPILE,$(TEST_IMPL_FILES),$(TEST_FULLNAME))
	./$(TEST_FULLNAME)

compile:
	$(call COMPILE,$(PROG_IMPL_FILES),$(PROG_FULLNAME))

clean:
	rm -rf $(TEST_FULLNAME) $(PROG_FULLNAME)