CC=gcc
CFLAGS=-I.

makebase: main.o tree.o
	$(CC) -o mallocdrv main.o tree.o

main.o: main.c tree.h malloc.h
	$(CC) -c -o main.o main.c

tree.o: tree.c tree.h malloc.h
	$(CC) -c -o tree.o tree.c

clean:
	rm -rf *.o mallocdrv
