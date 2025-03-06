CC = gcc
CFLAGS = -Wall -O2 -Werror=implicit-function-declaration -std=c99
EXEC_NAME = main.x

all: main.o
	$(CC) $(CFLAGS) -lsodium -o $(EXEC_NAME) main.o
	rm main.o

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

clean: 
	rm -f *.o *.x
