CC = gcc
CFLAGS = -Wall -std=c99

all: dynamicArray

dynamicArray: dynamicArray.o
	$(CC) $(CFLAGS) -o dynamicArray dynamicArray.o

dynamicArray.o: dynamicArray.c
	$(CC) $(CFLAGS) -c dynamicArray.c
	
clean:
	rm *.o dynamicArray
