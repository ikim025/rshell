VPATH = src
FLAGS = -ansi -pedantic -Wall -Werror -std=c++11
CC = g++
C11 = source/ opt/rh/devtoolset-2/enable

all: rshell.o ls.o


rshell.o: rshell.cpp
	#source /opt/rh/devtoolset-2/enable
	mkdir bin || [-d bin]
	$(CC) $(FLAGS) src/rshell.cpp -o bin/rshell

ls.o: ls.cpp
	$(CC) $(FLAGS) src/ls.cpp -o bin/ls

clean:
	rm -rf bin
