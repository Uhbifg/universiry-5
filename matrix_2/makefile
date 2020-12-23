CC=gcc
CFLAGS=-c -Wall

all: a clean

a: main.o
	$(CC) main.o matrix_init.c matrix_print.c matrix_inverse.c norm.c synchronize.c  -o a  -lpthread -lm 

matrix_print.o: matrix_print.c
	$(CC) matrix_print.c -lpthread
clean:
	rm -rf *.o
