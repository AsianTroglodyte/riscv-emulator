

CC=gcc

CFLAGS= -Wall -Wextra -std=c11


main: main.c rv64i.c
	$(CC) main.c rv64i.c -o main


test: test.c rv64i.c
	$(CC) test.c rv64i.c ./unity/src/unity.c -o test


# main_real: main.c rv64i.c
# 	$(CC) $(CFLAGS) main.c rv64i.c -o main
#
