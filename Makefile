

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

.PHONY: test
test: $(TEST_BIN)
	$(TEST_BIN)

$(TEST_BIN): tests.c rv64i.c rv64i.h | build
	$(CC) $(CFLAGS) tests.c rv64i.c ./unity/src/unity.c -o $(TEST_BIN)

comp_test: tests.c rv64i.c rv64i.h | build
	$(CC) $(CFLAGS) tests.c rv64i.c ./unity/src/unity.c -o $(TEST_BIN)

# make sure that the build file exists for tests and main
build:
	mkdir -p build

# debug
debug_test: tests.c rv64i.c
	$(CC) tests.c rv64i.c ./unity/src/unity.c -o ./build/tests

clean:
	rm ./build/tests main
