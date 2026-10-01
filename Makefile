

CC=gcc

CFLAGS= -Wall -Wextra -std=c11


main: main.c rv64i.c
	$(CC) main.c rv64i.c -o main

# main_real: main.c rv64i.c
# 	$(CC) $(CFLAGS) main.c rv64i.c -o main
