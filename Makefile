CC=gcc
CFLAGS=-Wall -g3
LDFLAGS=-lm
SRC_DIR=src
BUILD_DIR=build
BUILD_FILES=main io_helpers/io_helpers
BUILD_FILES_PATHS=$(shell for WORD in $(BUILD_FILES); do echo $(BUILD_DIR)/$(SRC_DIR)/$${WORD}.o;done)


.PHONY: create_builddir clean

build: create_builddir program.out 

program.out: $(BUILD_FILES_PATHS)
	$(CC) $^ -o $(BUILD_DIR)/$@	$(LDFLAGS)

$(BUILD_FILES_PATHS): $(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

create_builddir:
	$(shell find $(SRC_DIR) -type d | while read LINE; do mkdir -p $(BUILD_DIR)/$${LINE} ;done)

clean:
	-rm -rf build

$(BUILD_DIR)/$(SRC_DIR)/main.o $(BUILD_DIR)/$(SRC_DIR)/io_helpers/io_helpers.o: $(SRC_DIR)/io_helpers/io_helpers.h



