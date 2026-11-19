

CC=gcc
CFLAGS= -Wall -Wextra -Wno-unused-parameter -Wconversion -Wno-sign-conversion -std=c23 -fsanitize=address,undefined -fno-omit-frame-pointer
DEBUG= -Wall -Wextra -std=c23 -g -O0


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
.PHONY: debug
DEBUG_TEST_BIN=./build/debug_test
debug: $(DEBUG_TEST_BIN)
$(DEBUG_TEST_BIN): tests.c rv64i.c rv64i.h ./unity/src/unity.c | build
	$(CC) $(DEBUG) tests.c rv64i.c ./unity/src/unity.c -o $(DEBUG_TEST_BIN)


clean:
	rm -f $(DEBUG_TEST_BIN) $(TEST_BIN) $(MAIN_BUILD)
