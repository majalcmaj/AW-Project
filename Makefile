CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lm
BUILD_DIR=build
BUILD_FILES=$(BUILD_DIR)/src/main.o

.PHONY: create_builddir clean

build: create_builddir program.out 

program.out: $(BUILD_FILES)
	$(CC) $^ -o $(BUILD_DIR)/$@	$(LDFLAGS)

$(BUILD_FILES): $(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

create_builddir:
	$(shell find src -type d | while read LINE; do mkdir -p $(BUILD_DIR)/$${LINE} ;done)

clean:
	-rm -rf build

