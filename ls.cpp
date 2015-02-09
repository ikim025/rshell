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

void ls_l(char *loc){	
	DIR *md;
	struct dirent *mf;
	struct stat ms;
	md = opendir(loc);
	while((mf = readdir(md)) != NULL){
	}
	closedir(md);
	return;
}

int main(int argc, char **argv){
	char cmd[] = "bin";
	ls_l(cmd);
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
