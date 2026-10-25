

CC=gcc
CFLAGS= -Wall -Wextra -Wno-unused-parameter -Wconversion -Wno-sign-conversion -std=c11 -fsanitize=address,undefined -fno-omit-frame-pointer
DEBUG= -Wall -Wextra -std=c11 -g -O0


# main build
MAIN_BUILD=./build/main

.PHONY: main
main: $(MAIN_BUILD)

$(MAIN_BUILD): main.c rv64i.c rv64i.h | build
	$(CC) $(CFLAGS) main.c rv64i.c -o $(MAIN_BUILD)


# tests
TEST_BIN=./build/tests

## build and run test
.PHONY: test
test: $(TEST_BIN)
	$(TEST_BIN)

## only build test
.PHONY: build-tests
build-tests: $(TEST_BIN)

## build test recipe
$(TEST_BIN): tests.c rv64i.c rv64i.h | build
	$(CC) $(CFLAGS) tests.c rv64i.c ./unity/src/unity.c -o $(TEST_BIN)


# make sure that the build directory exists for tests and main
build:
	mkdir -p build

# build debug
debug_test: tests.c rv64i.c
	$(CC) tests.c rv64i.c ./unity/src/unity.c -o ./build/debug_test

clean:
	rm -f ./build/tests ./build/main
