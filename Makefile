VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror
CC = g++

all: rshell.o ls.o

rshell.o: rshell.cpp
	mkdir bin || [-d bin]
	$(CC) $(FLAGS) src/rshell.cpp -o bin/rshell

ls.o: ls.cpp
	$(CC) $(FLAGS) src/ls.cpp -o bin/ls

clean:
	rm -rf bin
