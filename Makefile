

CC=gcc

CFLAGS= -Wall -Wextra -std=c11

TEST_BIN=tests

main: main.c rv64i.c
	$(CC) main.c rv64i.c -o ./build/tests

test: $(TEST_BIN)
	./build/$(TEST_BIN)

$(TEST_BIN): tests.c rv64i.c
	$(CC) tests.c rv64i.c ./unity/src/unity.c -o ./build/tests

comp_test: tests.c rv64i.c
	$(CC) tests.c rv64i.c ./unity/src/unity.c -o ./build/tests

clean:
	rm tests main
