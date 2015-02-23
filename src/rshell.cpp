#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Friends advised me for a global variables.
char A[312];
char B[312];
int CTR1 = 0;
int CTR2 = 0;
int TRCK1 = 0;
int TRCK2 = 0;

//remove the lines before the file name for io redirection
char* dwhite(char *n){
	char d[] = {" \n\t<>"};
	int i = strcspn(n,d);
	while(i == 0){
		n++;
		i = strcspn(n,d);
	}
	return strtok(n,d);
}

//io redirection/ check
void iod(char* buffer){
	char *temp = buffer;
	while(*temp != '\0'){
		if(*temp == '<'){
			*temp = '\0';
			CTR1 = 1;
			temp ++;
			strcpy(A, dwhite(temp));
		}
		//wrong
		else if(*temp == '>'){
			*temp = '\0';
			CTR2 = 1;
			temp ++;
			strcpy(B,dwhite(temp));
		}
		//if(*temp == '|'){
		
		//}
		//else
	temp++;
	}
}

//
void com(char* b, char* arg[]){
	int i = 0;
	char d[] = " \t\n";
	char* counter = strtok(b,d);
	char* temp = NULL;
	while(counter != temp){
		arg[i++] = counter;
		//counter ++;
		counter = strtok(temp,d);
	}
	arg[i] = temp;
}

//opening files
void opf(){
	if(CTR1 == 1){
		TRCK1 = open(A, O_RDONLY);
		if(TRCK1 == -1)
			perror("open");
	}
	if(CTR2 == 1){
		TRCK2 = open(B, O_RDWR | O_CREAT, 0666);
		if(TRCK2 == -1)
			perror("open");
	}
}

//closing files
void clf(){
	if(TRCK1 > 0){
		if(close(TRCK1) == -1)
			perror("close");
	}
	if(TRCK2 > 0){
        	if(close(TRCK2) == -1)
                	perror("close");
        }
}

void cw(char* arg[], int pi[]){
	if(TRCK1 > 0){
		if(dup2(TRCK1,STDIN_FILENO) < 0)
			perror("dup2");
		close(TRCK1);
	}
	if(TRCK2 > 0){
		if(dup2(TRCK2,STDOUT_FILENO) < 0)
			perror("dup2");
		close(TRCK2);
	}
	if(execvp(arg[0],arg) < 0)
		perror("execution");
	exit(1);
}

int main(){
	char b[312];
	char * arg[100];

	//set up the global
	CTR1 = 0;
	CTR2 = 0;
	TRCK1 = 0;
	TRCK2 = 0;
	while(1){	
		fputs("$ ", stdout);
		fgets(b, 312, stdin);
	        iod(b);
		com(b,arg);
		opf();
      		int pi[2];
        	if( pipe(pi) < 0)
                	perror("pipe");
        	
		//fork
		int pid = fork();
        	switch(pid){
                	case -1:
                       	 perror("fork");
                      	 break;
                	case 0:
                         cw(arg,pi);
                         break;
                	default:
                         if(wait(0) == -1)
                                 exit(-1);
                                 break;
        	}
		CTR1 = 0;
		CTR2 = 0;
		TRCK1 = 0;
		TRCK2 = 0;
		clf();
	}
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
