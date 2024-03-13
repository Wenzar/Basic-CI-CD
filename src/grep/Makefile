CC=gcc
CFLAGS=-Wall -Wextra -Werror -c


all: build clean
build: s21_grep

s21_grep: grep.o 
	$(CC) grep.o -o s21_grep

grep.o: grep.c
	$(CC) $(CFLAGS) grep.c

clean:
	rm -rf *.o 
