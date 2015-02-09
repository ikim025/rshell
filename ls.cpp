#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <dirent.h>

using namespace std;

void ls_a(char *loc){	
	DIR *md;
	struct dirent *mf;
	struct stat ms;
	md = opendir(loc);
	if(md == NULL){
		perror("opendir X");
		exit(1);	
	}
	while((mf = readdir(md)) != NULL){
		stat(mf -> d_name, &ms);
		struct stat tp;
		stat(mf -> d_name, &tp);
		if(S_ISDIR(tp.st_mode)){
			printf("%s ", mf -> d_name);
			std :: cout << "/";
		}
		else
			printf(" %s ", mf -> d_name);
	}
	std :: cout << "\n";
	closedir(md);
	return;
}

int main(int argc, char **argv){
	char cmd[] = "bin";
	ls_a(cmd);
	/*char cmd[512];
	int ending = 0;
	do{
	printf("$ ");
	cin >> cmd;
	if(cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't')
		ending = 1;
	}while(ending == 0);
	return 0;
*/
}
