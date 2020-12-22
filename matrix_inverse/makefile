CC=gcc
CFLAGS=-c -Wall

all: a clean

a: main.o matrix_print.o matrix_init.o matrix_eginvalues.o norm.o
	$(CC) main.o matrix_init.o matrix_print.o matrix_eginvalues.o norm.o -o a -lm

main.o: main.c
	$(CC) $(CFLAGS) main.c

matrix_print.o: matrix_print.c
	$(CC) $(CFLAGS) matrix_print.c

matrix_init.o: matrix_init.c
	$(CC) $(CFLAGS) matrix_init.c

matrix_eginvalues.o: matrix_eginvalues.c
	$(CC) $(CFLAGS) matrix_eginvalues.c

norm.o: norm.c
	$(CC) $(CFLAGS) norm.c

clean:
	rm -rf *.o
