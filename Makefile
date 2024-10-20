PROG_OUT := torrent.o
PROG_DIR := src
LIB_DIR := $(PROG_DIR)/lib
PROG_FULLNAME := $(PROG_DIR)/$(PROG_OUT)
PROG_IMPL_FILES := \
$(wildcard $(PROG_DIR)/*.cpp \
	$(LIB_DIR)/*/*.cpp \
 	$(PROG_DIR)/commands/*.cpp)

TEST_OUT := test.o
TEST_DIR := $(PROG_DIR)/tests
TEST_FULLNAME := $(TEST_DIR)/$(TEST_OUT)
TEST_IMPL_FILES := \
$(wildcard $(TEST_DIR)/*_tests.cpp \
	$(LIB_DIR)/*/*.cpp)

CC := g++
CFLAGS := -Wall -g -I$(PROG_DIR)

define COMPILE =
$(CC) $(1) -o $(2) $(CFLAGS);
endef

all: clean

debug:
	echo "there is nothing to debug"

test:
	$(call COMPILE,$(TEST_IMPL_FILES),$(TEST_FULLNAME))
	./$(TEST_FULLNAME)

compile:
	$(call COMPILE,$(PROG_IMPL_FILES),$(PROG_FULLNAME))

clean:
	rm -rf $(TEST_FULLNAME) $(PROG_FULLNAME)