#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]){
    string cmd;
    int pid;
    while(cmd != "exit"){
        std::cout << "$ ";
        cin >> cmd;

        pid = fork();
        if(pid > 0){
            waitpid(pid,0,0);
        }
        else if(pid == 0){
            std::cout << "This is the child process ";
            if(-1 == execvp(argv[0], argv))
                perror("There was an error in execvp. ");
            exit(1);
        }
        else if(pid == -1){
            perror("There was an error with for(). ");
            exit(1);
        }
    }

    return 0;
}
