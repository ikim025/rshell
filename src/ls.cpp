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
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

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


void ls(const char *loc, bool dasha){
	DIR *md;
	md = opendir(loc);
	struct dirent *mf;
	bool fold;
	if(md == NULL){
		perror("opendir X");
		exit(1);
	}
	while((mf = readdir(md))){
		if(!dasha){
			if(boost::starts_with(mf -> d_name, ".") || boost::starts_with(mf -> d_name, ".."))
				continue;
		}
		printf(" %s", mf -> d_name);
		
		if((fold = mf->d_type & DT_DIR))
			cout << "/";
	}
	if(closedir(md) == -1){
		perror("closedir X");
		exit(1);
	}
	cout << endl;
}

int main(int argc, char **argv){
	char loc[] = "bin";
	int ending = 0;
	char check[512];
	while(ending == 0){
		std :: cout << "$ ";
		cin >> check;
		if(check[0] == 'l' && check[1] == 's')
			ls(loc,false);
		else if(check[0] == 'e' && check[1] == 'x' && check[2] == 'i' && check[3] == 't')
			ending = 1; 
		else
			ls(loc,true);
	}
	return 0;

}
