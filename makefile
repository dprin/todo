CC = g++
CFLAGS  = -g -Wall --std=c++17 -lstdc++fs

default: compile

compile: parser.cpp main.cpp
	$(CC) $(CFLAGS) -c main.cpp parser.cpp 
	$(CC) $(CFLAGS) -o main parser.o main.o
	$(RM) *.o

install:
	make compile
	cp ./main /usr/bin/todo

clean:
	$(RM) main *~