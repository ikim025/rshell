#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(){
	return 0;
}




/*
using namespace std;

void parse(char *cd, char **par){
    for(int i = 0; i < 512; i++){
        par[i] = strsep(&cd, " ");
        if(par[i] == NULL)
            break;
    }
}

int ex(char** par){
    int pid = fork();
    if(pid == -1){
        perror("There was an error with fork(). ");
        exit(1);
    }
    else if(pid == 0){
        execvp(par[0], par);
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
        cin >> cmd;
        if(cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = '\0';
        parse(cmd, par);
        if((cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't') || ex(par) == 0)
            ending = 1;
    }
    return 0;






///////////////////////////hw2


}*/
