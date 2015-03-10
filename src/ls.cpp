#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <locale>
#include <sys/stat.h>
#include <string>
#include <algorithm>
#include <cstring>
#include <errno.h>
#include <vector>
#include <libgen.h>
#include <cstdio>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>


#include <pwd.h>
#include <grp.h>
#include <time.h>
using namespace std;

//GLOBAL
bool LFLAG = false;
bool AFLAG = false;
bool RFLAG = false;

void arl(char *temp,vector<bool> &arl){
	if(strchr(temp,'a') != NULL){ arl[0] = true; AFLAG = true; }
	if(strchr(temp,'r') != NULL){ arl[1] = true; RFLAG = true; }
        if(strchr(temp,'l') != NULL){ arl[2] = true; LFLAG = true; }

}

void flagc(int argc, char *argv[], vector<char *> &f, vector<char *> &d, vector<bool> &flag){
	char *temp;
	for(int i = 1; i < argc; i++){
		temp = argv[i];
		if(temp[0] == '-'){
			arl(temp,flag);
		} 
		else{
			struct stat temp2;
			if(stat(temp, &temp2) == -1){
				perror("stat");
				exit(1);
			}
			else if(S_ISDIR(temp2.st_mode)) d.push_back(temp);
			else f.push_back(temp);
		}
	}
}


void diropen(char *dir, DIR *md){
	md = opendir(dir);
	if(md == NULL){
		perror("opendir");
		exit(1);
	}
}


void print(char *dir, vector<bool> &flag){
	DIR *md = opendir(dir);
	//DIR *md;
	struct dirent *mf;
	//struct stat ms;
	//diropen(dir,md);
	while((mf = readdir(md))){
		string curr(mf -> d_name);
		if(!flag.at(0)){
			if(boost:: starts_with(curr, ".") || boost :: starts_with(curr, ".."))
				continue;
		}
		bool isdir = (mf -> d_type & DT_DIR);
		cout << curr;
		if(isdir) cout << "/";
		cout << "\t";
	}
	if(closedir(md) == -1){
		perror("closedir");
		exit(1);
	}
	cout << endl << endl;
}

void l_flag(char *dir,vector<bool> &flag){
	//DIR *md;
	DIR *md = opendir(dir);
	struct dirent *mf;
	struct stat ms;
	//diropen(dir,md);
	while((mf = readdir(md))){
		/*if(stat(mf->d_name, &ms) == -1){
                        perror("stat");
                        exit(1);
                }*/
		string path = dir;	
		path += "/";
		path += mf -> d_name;
		stat(path.c_str(), &ms);
		//string curr = mf -> d_name;
		string curr (mf -> d_name);
		
		if(!flag.at(0)){
                       if(boost:: starts_with(curr, ".") || boost :: starts_with(curr, ".."))
                                continue;
                }
/*	
		if(stat(mf->d_name, &ms) == -1){
			perror("stat");
			exit(1);
		}
*/			
	
	
		if (S_ISDIR(ms.st_mode)) cout << 'd';
		else if(S_ISLNK(ms.st_mode)) cout << "l";
		else cout << "-";

		cout << ((ms.st_mode & S_IRUSR) ? 'r' : '-') << ((ms.st_mode & S_IWUSR) ? 'w' : '-')
		     << ((ms.st_mode & S_IXUSR) ? 'x' : '-') << ((ms.st_mode & S_IRGRP) ? 'r' : '-')
		     << ((ms.st_mode & S_IWGRP) ? 'w' : '-') << ((ms.st_mode & S_IXGRP) ? 'x' : '-')
		     << ((ms.st_mode & S_IROTH) ? 'r' : '-') << ((ms.st_mode & S_IWOTH) ? 'w' : '-')
	             << ((ms.st_mode & S_IXOTH) ? 'x' : '-') << " " << ms.st_nlink << " "; 
		
		
		struct passwd *pass = getpwuid(ms.st_uid);
		struct group *grp = getgrgid(ms.st_gid);
		if(grp == NULL){
			perror("getgrgid");
		}
		if(pass == NULL){
			perror("getpwuid");
		}
		char s[1000];
		time_t t = time(NULL);





			cout << pass -> pw_name << " "<< grp -> gr_name << right << setw(8) 
		     << ms.st_size << " ";
		if(strftime(s,sizeof(s), "%A, %c", localtime(&t))) 
			cout << s;

			cout  << " " <<  mf-> d_name << endl;
	}
	if(closedir(md) == -1){
		perror("closedir");
		exit(1);
	}
}



int main(int argc, char * argv[]){
	LFLAG = false;
	AFLAG = false;
	RFLAG = false;
	vector<char *> file;
	vector<char *> di;
	vector<char *>fold;
	vector<bool> flag(3,false);

        flagc(argc, argv, file, di, flag);



	char *temp;
        for(int i = 1; i < argc; i++){
                temp = argv[i];
                if(temp[0] != '-'){
                       fold.push_back(temp);
                }
	}
	if(fold.size() != 0){
		for(unsigned i = 0; i < fold.size(); i++){
			if(LFLAG) l_flag(fold.at(i),flag);
			else print(fold.at(i),flag);
		}
	}
	else{
	char dir[] = ".";
	(flag.at(2)) ? l_flag(dir,flag) : print(dir,flag);
	}
/*
	vector<char *> input;
	vector<string>d;
	int i = 1;
	while(i < argc){ input.push_back(argv[i]); i++; }
	int counter = 0;
	int track = 0;
	while(track < input.size()){
		if(strcmp(input[track], "-a") == 0){
			AFLAG = true;
			counter++;
		}
		else if(strcmp(input[track], "-l") == 0){
			LFLAG = true;
			counter ++;
		}
		else if(strcmp(input[track], "-R") == 0){
			RFLAG = true;
			counter ++;
		}
		else if(strcmp(input[track], "-al") == 0 || strcmp(input[track], "-la") == 0){
			AFLAG = true; 
			LFLAG = true;
			counter ++;
		}
                else if(strcmp(input[track], "-Rl") == 0 || strcmp(input[track], "-lR") == 0){
                        RFLAG = true;
                        LFLAG = true;
                        counter ++;
                }
                else if(strcmp(input[track], "-aR") == 0 || strcmp(input[track], "-Ra") == 0){
                        AFLAG = true;
                        RFLAG = true;
                        counter ++;
                }
		else if(strcmp(input[track], "-alR") == 0 || strcmp(input[track], "-laR") == 0 ||
			strcmp(input[track], "-Ral") == 0 || strcmp(input[track], "-Rla") == 0 ||
			strcmp(input[track], "-lRa") == 0 || strcmp(input[track], "-aRl") == 0){
			AFLAG = true;
			LFLAG = true;
			RFLAG = true;
			counter ++;
		}
		track ++;
	}
	while(counter < input.size){
		d.push_back(string(input[counter]));
		counter ++;
	}
	if(d.size() == 0) d.push_back(".");
*/

	//if(flag.at(0)) cout << " a true" << endl;
	//if(flag.at(1)) cout << " r true" << endl;
	//if(flag.at(2)) cout << " l true\n" ;
	//if(flag.at(2))
	//	l_flag(dir);
	//else print(dir, flag);
	return 0;
}
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
/*

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

}*/
