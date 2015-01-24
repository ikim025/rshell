#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void parse(char *cd, char **par){
    for(int i = 0; i < 512; i++){
        par[i] = strsep(&cd, " ");
        if(par[i] == NULL)
            break;
    }
}

int ex(char** params){
    int pid = fork();
    if(pid == -1){
        perror("There was an error with fork(). ");
        exit(1);
    }
    else if(pid == 0){
        execvp(params[0], params);
        return 0;
    }
    else{
        waitpid(pid, 0, 0);
        return 1;
    }
}

int main(int argc, char **argv){
    char cmd[512];
    char *par[512];

    int ending = 0;
    while(ending == 0){
        std :: cout << "$ ";
        if(fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;
        if(cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) -1] = '\0';
        parse(cmd, par);
        if(strcmp(par[0], "exit")  == 0)
            ending = 1;
        if(ex(par) == 0)
            ending = 1;
    }
    return 0;


}
