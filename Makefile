VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror
CC = g++

all: rshell.o

rshell.o: rshell.cpp
	$(CC) $(FLAGS) rshell.cpp

clean:
	rm -rf *o rshell

