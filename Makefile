VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror
CC = g++

all: rshell.o

rshell.o: rshell.cpp
	mkdir bin || [-d bin
	$(CC) $(FLAGS) rshell.cpp -o bin/rshell

clean:
	rm -rf bin
