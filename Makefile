VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror
CC = g++

all: ls.o

rshell.o: rshell.cpp
	mkdir bin || [-d bin]
	$(CC) $(FLAGS) rshell.cpp -o bin/rshell

ls.o: ls.cpp
	mkdir bin || [d bin]
	$(CC) $(FLAGS) ls.cpp -o bin/ls

clean:
	rm -rf bin
