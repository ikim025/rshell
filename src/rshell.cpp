#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

using namespace std;

string curr_d(){
    char currd[BUFSIZ];
    getcwd(currd,sizeof(currd));
    //if(!getcwd(currd,sizeof(currd)) != NULL) perror("getcwd");
    if(currd == NULL) perror("getcwd");
    return currd;
}

char* dwhite(char *n){
    char d[] = {" \n\t<>"};
    int i = strcspn(n,d);
    while(i == 0){
        n++;
        i = strcspn(n,d);
    }
    return strtok(n,d);
}

void handler(int sig){
    if(signal(SIGINT, SIG_IGN) == SIG_ERR)
        perror("SIGINT");
    else if(signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        perror("signal Z");
    if(kill(getpid(), SIGTSTP) == -1)
        perror("kill");
}


int check(char *ptr){
    return (*ptr >= '0' && *ptr <= '9' );
}


int red( char *input, char **argv) {
    
    int flag;
    int in = 0;
    int out;
    char *file, *ptin, *ptout;
    char tin[1024];
    char tout[1024];
    char fileo[1024];
    char ms[1024];
    char tracker[2];
    int trr =0;
    
    char *buf = input;
    for(int i = 0; i < 10; i++){
        flag = 0;
        out = 1;
        if ( (ptin = strstr(buf, "<")) != NULL ) {
            file = NULL;
            strcpy(tin, ptin+1);
            file = strtok(tin," \n\t<>");
            if ( file == NULL ) {
                cout << "error" << endl;
            }
            if ( check(ptin - 1)){
                char value[2];
                strncpy(value, ptin - 1, 1);
                value[1] = 0;
                in = atoi(value);
                *(ptin -1) = ' ';
            }
            buf = ptin +1;
            int dp = -1;
            
          
            if ( file != NULL )
                if((dp = open (file, O_RDONLY)) == -1) perror("open");
            

            if ( dp != -1 ) {
                if((close ( in )) == -1) perror("close");
                if((dup( dp )) == -1) perror("dup");
                if((close ( dp )) == -1) perror("close");
            }
        }
        
        if ( (ptout = strstr(buf, ">")) != NULL){
            if(*(ptout + 1 ) == '>'){
                strcpy(tout, ptout+2);
                flag = 1;
                *ptout = 0;
                buf = ptout +2;
            }
            else{
                strcpy(tout, ptout+1);
                buf = ptout + 1;
            }
            *ptout = 0;
            char *pbuf;
            pbuf = strtok(tout, " \t\n<>");
            strcpy(fileo, pbuf);
            if ( check(ptout - 1)){
                char value[2];
                strncpy(value, ptout - 1, 1);
                value[1] = 0;
                out = atoi(value);
                *(ptout -1) = ' ';
            }
            int cust = 0;
            
            if(flag == 0){
                if((cust = open(fileo, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) perror("open");
            }
            else if(flag == 1){
                if((cust = open(fileo, O_WRONLY | O_CREAT | O_APPEND , S_IRWXU | S_IRWXG | S_IRWXO)) == -1) perror("open");
            }
            if( cust != -1){
                if((close(out)) == -1) perror("close") ;
                if((dup( cust)) == -1) perror("dup");
                if((close(cust)) == -1) perror("close");
            }
            
        }
        else if ( (ptin = strstr(buf, "<<<")) != NULL){
            char *istart, *ilast;
            istart = strstr(ptin +3 , "\"" );
            ilast = strstr(istart +1, "\"");
            strncpy(ms, istart+1, ilast-istart-1);
            ms[ilast-istart-1] = 0;
            trr = 1;
        }
        if ( ptin  != NULL ) *ptin  = 0;
        if ( ptout != NULL ) *ptout = 0;
    }
    
    if(trr == 1){
        int pip[2];
        if((pipe(pip)) == -1)perror("pipe");
        int nval;
        nval = write(pip[1], ms, strlen(ms));
        if(nval < 0)
            perror("write");
        strcpy(tracker, "\n");
        tracker[1] = 0 ;
        nval = write(pip[1], tracker, strlen(tracker));
        if(nval < 0)
            perror("write ");
        
        if((dup2(pip[0], 0)) == -1) perror("dup2");
        if((close(pip[1])) == -1) perror("close");
    }
    
    int final;
    int index = 0;
    argv[index] = strtok(input, " \t\n");
    while(argv[index] != NULL){
        index ++;
        argv[index] = strtok(NULL, " \t\n");
    }
    argv[index +1] = NULL;
    final = index + 1;
    return final;
}

int parse(char *input, char **argv){
    char **command = new char *[50];
    char *hp;
    int nc = 0;
    int tr;
    int background_flag;
    
    char * cur_pt;
    background_flag = 0;
    if((cur_pt = strstr(input, "&")) != NULL){
        background_flag = 1;
        *cur_pt = ' ';
    }
    
    hp = input;
    tr  = 0;
    int nlen = strlen(input);
    for( int i=0; i < nlen ; i++) {
        if(strncmp (&input[i], "|", 1 ) == 0 ) {
            command[tr] = hp;
            hp = &input[i+1];
            input[i] = 0;
            tr++;
        }
    }
    command[tr] = hp;
    nc = tr;
    
    int pid;
    int st;
    int npip[2];
    int  pip[2];
    st = 0;
    
    if(nc == 0){
        
        pid = fork ();
        if ( pid == -1 ) {
            perror("fork");
        } else if ( pid == 0 ) {
            red( command[0], argv );
            if ( execvp ( argv[0], argv ) == -1 )
                perror("execvp" );
            exit(errno);
        } else if ( pid > 0) {
            if(background_flag == 0){
                if((wait( &st )) == -1) perror("wait");
                return st;
            }
            else{
                return 0;
            }
        }
        
    }
    else{
        if((pipe(pip)) == -1) perror("pipe");
        switch( pid = fork()){
            case 0:
                red( command[0], argv );
                
                if((dup2( pip[1], 1)) == -1) perror("dup2");
                if((close( pip[0] )) == -1) perror("close");
                
                if ( execvp( argv[0], argv ) == -1 )
                    perror( "execvp ");
                exit( errno );
            case -1:
                perror("fork ");
                exit(1);
        }
        for(int j= 1; j < nc ; j++){
            if((pipe(npip)) == -1) perror("pipe");
            switch( pid = fork()){
                case 0:
                    red( command[j], argv );
                    
                    if((dup2( pip[0], 0)) == -1) perror("dup2");
                    if((close( pip[1] )) == -1) perror("close");
                    if((dup2( npip[1], 1)) == -1) perror("dup2");
                    if((close( npip[0])) == -1) perror("close");
                    
                    if ( execvp( argv[0], argv ) == -1 )
                        perror( "exevp ");
                    exit( errno );
                case -1:
                    perror("fork ");
                    exit(1);
            }
            if((close( pip[0])) == -1) perror("close");
            if((close( pip[1])) == -1) perror("close");
            pip[0] = npip[0];
            pip[1] = npip[1];
        }
        switch( pid = fork()) {
            case 0:
                red( command[nc], argv );
                
                if((dup2( pip[0], 0)) == -1) perror("dup2");
                if((close( pip[1] )) == -1) perror("close");
                
                if ( execvp ( argv[0], argv ) == -1 )
                    perror( "exevp ");
                exit ( errno );
            case -1:
                perror("fork ");
                exit(1);
        }
        if((close( pip[0] )) == -1) perror("close");
        if((close( pip[1] )) == -1) perror("close");
        if(background_flag == 0){
            while((pid = wait(&st)) != -1)
                if(pid == -1) perror("wait");
            return st;
        }
        else{
            return 0;
        }
    }
    
    return st;
}

int main(){
    
    if(signal(SIGINT, handler) == SIG_ERR){
        perror("CTRL-C");
    }
    if(signal(SIGTSTP, handler) == SIG_ERR){
        perror("CTRL-Z");
    }
    char *user = getlogin();
    char host[100];
    gethostname(host,sizeof host);
    
    string path = curr_d();
    
    while(1){
        path = curr_d();
        std :: cout << user << "@" << host
        << ":" << path << "$ ";
        
        char cmd[1024];
        char buf[1024];
        char * hp, * next;
        int mystat;
        bool stat1;
        
        cin.getline(cmd,1024);
        
        unsigned int i = 0;
        while(i < strlen(cmd)){
            if(cmd[i] == '#'){
                cmd[i] = '\0';
                break;
            }
            i++;
        }
        
        if(!strcmp(cmd, "exit")) exit(1);
        
        char ** argv;
        
        argv = new char *[50];
        
        if(strstr(cmd,";") != NULL){
            hp = cmd;
            next = strstr(hp,";");
            while(next != NULL){
                for(int i =0; i < 1024; i++) buf[i]=0;
                strncpy(buf,hp,next-hp);
                mystat = parse(buf, argv);
                hp = next + 1;
                next = strstr(hp,";");
            }
            
            for(int i =0; i < 1024; i++) buf[i]=0;	
            strcpy(buf, hp);
            mystat = parse(buf, argv);
        }
        
        else if(strstr(cmd,"&&") != NULL){
            hp = cmd;
            stat1 = true;
            next = strstr(hp,"&&");
            while(next != NULL && stat1){
                for(int i =0; i < 1024; i++) buf[i]=0;
                strncpy(buf,hp,next-hp);	
                mystat = parse(buf, argv);
                if(mystat != 0) stat1 = false;
                hp = next+2;
                next = strstr(hp,"&&");
            }
            if(stat1){
                for(int i =0; i < 1024; i++) buf[i]=0;	
                strcpy(buf, hp);
                mystat = parse(buf, argv);
            }
        }
        
        else if(strstr(cmd,"||") != NULL){
            hp = cmd;
            stat1 = true;
            next = strstr(hp,"||");
            while(next != NULL && stat1){
                for(int i =0; i < 1024; i++) buf[i]=0;
                strncpy(buf,hp,next-hp);	
                mystat = parse(buf, argv);
                if(mystat == 0) stat1 = false;
                hp = next+2;
                next = strstr(hp,"||");
            }
            if(stat1){
                for(int i =0; i < 1024; i++) buf[i]=0;	
                strcpy(buf, hp);
                mystat = parse(buf, argv);
            }
        }
        
        else{
            mystat = parse(cmd,argv);
        }
        
        delete[] argv;
    }
    return 0;
}
