CC=gcc
CFLAGS= -Wall

all: main.o sem.o tasks.o
	$(CC) $(CFLAGS) main.o sem.o tasks.o -o test2
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
sem.o:	sem.c
	$(CC) $(CFLAGS) -c sem.c -o sem.o
tasks.o:  tasks.c
	$(CC) $(CFLAGS) -c tasks.c -o tasks.o
clean:	
	rm -f *.o

