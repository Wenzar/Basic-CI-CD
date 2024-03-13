CC=gcc
CFLAGS=-Wall -Wextra -Werror -c
COMMON = ../common/

all: build clean
build: s21_cat

s21_cat: cat.o TablesCat.o 
	$(CC) cat.o TablesCat.o -o s21_cat

cat.o: cat.c
	$(CC) $(CFLAGS) cat.c

TablesCat.o: TablesCat.c
	$(CC) $(CFLAGS) TablesCat.c

clean:
	rm -rf *.o 
