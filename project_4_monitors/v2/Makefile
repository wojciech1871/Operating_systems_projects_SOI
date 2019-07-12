CC=g++
CFLAGS=-Wall
TARGET=fun

all: main.o
	$(CC) $(CFLAGS) -pthread main.o -o $(TARGET)
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o
clean:	
	rm -f *.o $(TARGET)

