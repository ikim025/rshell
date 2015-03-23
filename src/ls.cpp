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
#include <queue>
#include <ctype.h>
#include <cstdio>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <cctype>

#include <pwd.h>
#include <grp.h>
#include <time.h>
using namespace std;

//GLOBAL
bool LFLAG = false;
bool AFLAG = false;
bool RFLAG = false;

void arl(char *temp,vector<bool> &arl){
    if(strchr(temp,'a') != NULL){ AFLAG = true; }
    if(strchr(temp,'r') != NULL){ RFLAG = true; }
    if(strchr(temp,'l') != NULL){ LFLAG = true; }
    
}

void diropen(char *dir, DIR *md){
    md = opendir(dir);
    if(md == NULL){
        perror("opendir");
        exit(1);
    }
}


void mode_to_letters( int mode, char str[] ){
    strcpy( str, "----------" );
    if ( S_ISDIR(mode) )  str[0] = 'd';
    if ( S_ISCHR(mode) )  str[0] = 'c';
    if ( S_ISBLK(mode) )  str[0] = 'b';
    if ( S_ISLNK(mode) )  str[0] = 'l';
    if ( mode & S_IRUSR ) str[1] = 'r';
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';
    if ( mode & S_IRGRP ) str[4] = 'r';
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';
    if ( mode & S_IROTH ) str[7] = 'r';
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}

void l_flag(vector<string> list, int m){
    int max = 78;
    unsigned col = 0;
    unsigned line = 0;
    char format[1024];
    char space[2];
    strcpy(space, " ");
    if(!LFLAG){
        col = ((max-2) / (m+2));
        if(col == 0) col = 1;
        line = (list.size() + col - 1) / col;
        for(unsigned i = 0; i < line; i++){
            for(unsigned j = 0; j < col; j++){
                if(i + line * j < list.size()){
                    unsigned found = list.at(i + line * j).find_last_of("/");
                    strcpy(format, list.at(i + line * j).substr(found + 1).c_str());
                    struct stat mi;
                    if(stat(list.at(i + line * j).c_str(), &mi) == -1) perror ("stat");
                    if(mi.st_mode & S_IXUSR || mi.st_mode & S_IXGRP || mi.st_mode & S_IXOTH) printf("\x1b[32m");
                    if(S_ISDIR(mi.st_mode)) printf("\x1b[34m");
                    printf("%s", format);
                    printf("\x1b[0m");
                    if(j <= col - 1){
                        for(unsigned k = 0; k < m + 2 - strlen(format); k++){
                            printf("%s", space);
                        }
                    }
                }
            }
				printf("\n");
        }
    }
    else if(LFLAG){
        int total = 0;
        for(unsigned i = 0; i < list.size(); i++){
            struct stat mi;
            if(stat(list.at(i).c_str(), &mi) == -1) perror("stat");
            total += (int)mi.st_blocks;
        }
        cout << "total " << total << endl;
        for(unsigned i = 0; i < list.size(); i++){
            struct stat mi;
            if(lstat(list.at(i).c_str(), &mi) == -1) perror("lstat");
            else{
                char mode[11];
                char fname[1024];
                char lname[1024];
                
                struct passwd *pass;
                if((pass = getpwuid(mi.st_uid)) == NULL) perror("getpwuid");
                struct group *grp;
                if((grp = getgrgid(mi.st_gid)) == NULL) perror("getgrgid");
                
                
                strcpy( mode, "----------" );
                if ( S_ISDIR(mi.st_mode) )  mode[0] = 'd';
                if ( S_ISCHR(mi.st_mode) )  mode[0] = 'c';
                if ( S_ISBLK(mi.st_mode) )  mode[0] = 'b';
                if ( S_ISLNK(mi.st_mode) )  mode[0] = 'l';
                if ( mi.st_mode & S_IRUSR ) mode[1] = 'r';
                if ( mi.st_mode & S_IWUSR ) mode[2] = 'w';
                if ( mi.st_mode & S_IXUSR ) mode[3] = 'x';
                if ( mi.st_mode & S_IRGRP ) mode[4] = 'r';
                if ( mi.st_mode & S_IWGRP ) mode[5] = 'w';
                if ( mi.st_mode & S_IXGRP ) mode[6] = 'x';
                if ( mi.st_mode & S_IROTH ) mode[7] = 'r';
                if ( mi.st_mode & S_IWOTH ) mode[8] = 'w';
                if ( mi.st_mode & S_IXOTH ) mode[9] = 'x';
                
                printf( "%s"    , mode );
                printf( "%4d "  , (int) mi.st_nlink);
                printf( "%-8s " , pass -> pw_name);
                printf( "%-8s " , grp -> gr_name );
                printf( "%8ld " , (long)mi.st_size);
                printf( "%.12s ", 4+ctime(&mi.st_mtime));
                unsigned found = list.at(i).find_last_of("/");
                strcpy(fname, list.at(i).substr(found + 1).c_str());
                if(mi.st_mode & S_IXUSR || mi.st_mode & S_IXGRP || mi.st_mode & S_IXOTH) printf("\x1b[32m");
                if(S_ISDIR(mi.st_mode)) printf("\x1b[34m");
                printf( "%s", fname);
                if(S_ISLNK(mi.st_mode)){
                    ssize_t istat;
                    strcpy(fname,list.at(i).c_str());
                    istat = readlink(fname, lname, mi.st_size+1);
                    if(istat == -1) perror("readlink");
                    lname[istat] = '\0';
                    printf(" -> %s", lname);
                }
                printf( "\x1b[0m" );
                printf( "\n");
            }
        }
    }
}

void lss(char *dir, vector<string> * list, int *max, queue<string> * dirlist){
    DIR * curr;
    if((curr = opendir(dir)) == NULL){
        perror("opendir");
    }
    
    //diropen(dir,curr);
    int len;
    struct dirent * md;
    char buf[1024];
    while((md = readdir(curr)) != NULL){
        if(md -> d_name[0] != '.' || AFLAG){
            sprintf(buf, "%s/%s", dir, md -> d_name);
            list -> push_back(buf);
            len = strlen(md -> d_name);
            if(*max < len) *max = len;
        }
    }
    if(closedir(curr) == -1){
        perror("closdir");
        exit(1);
    }
}

void direc(vector<string> list, queue<string> *dirr){
    char buf[1024];
    char fname[1024];
    struct stat mi;
    
    for(unsigned i = 0; i < list.size(); i++){
        unsigned found = list.at(i).find_last_of("/");
        strcpy(fname,list.at(i).substr(found+1).c_str());
        sprintf(buf, "%s", list.at(i).c_str());
        if(stat(buf, &mi) == -1) perror("stat");
        if(S_ISDIR(mi.st_mode) && !(strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)) dirr -> push(buf);
    }
}

bool check(const string & str1, const string & str2){
    return strcasecmp(str1.c_str(), str2.c_str()) <= 0;
}

int main(int argc, char *argv[]){
    int index, c;
    int max = 0;
    vector<string> list;
    queue<string>dirr;
    opterr = 0;
    while((c = getopt(argc, argv, "alR")) != -1){
        switch(c){
          case 'a':
            AFLAG = 1;
            break;
          case 'l':
            LFLAG = 1;
            break;
          case 'R':
            RFLAG = 1;
            break;
          default:
            abort();
        }
    }
    if(optind == argc){
        char dir[2];
        strcpy(dir,".");
        lss(dir, &list, &max, &dirr);
    }
    else{
        for(index = optind; index < argc; index++){
            char buf[1024];
            struct stat mi;
            strcpy(buf,argv[index]);
            if(stat(buf, &mi))
                dirr.push(buf);
            else{
                int len;
                list.push_back(buf);
                len = strlen(buf);
                if(max < len) max = len;
            }
        }
    }
    sort(list.begin(), list.end(), check);
    l_flag(list, max);
    if(dirr.size() > 0 && !RFLAG){
        while(dirr.size() > 0){
            char dirp[1024];
            strcpy(dirp, dirr.front().c_str());
            dirr.pop();
            max = 0;
            list.clear();
            lss(dirp, &list, &max, &dirr);
            sort(list.begin(), list.end(), check);
            cout << endl << dirp << ":" << endl;
            l_flag(list, max);
        }
    }
    if(RFLAG){
        direc(list, &dirr);
        while(dirr.size() > 0){
            char dirp[1024];
            strcpy(dirp, dirr.front().c_str());
            dirr.pop();
            max = 0;
            list.clear();
            lss(dirp, &list, &max, &dirr);
            sort(list.begin(), list.end(), check);
            direc(list, &dirr);
            cout << endl << dirp << ":" << endl;
            l_flag(list, max);
        }
    }
    return 0;
}

