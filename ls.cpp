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
/*
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
		printf(" %s ", mf -> d_name);
	}
	std :: cout << "\n";
	closedir(md);
	return;
}*/

int main(int argc, char **argv){
	
	return 0;

}