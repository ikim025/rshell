#include <pwd.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>

using namespace std;

//global
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
boost::char_separator<char> conn(";&|<>");
boost::char_separator<char> space("\t\r\n\a ");




char* dwhite(char *n){
	char d[] = {" \n\t<>"};
	int i = strcspn(n,d);
	while(i == 0){
		n++;
		i = strcspn(n,d);
	}
	return strtok(n,d);
}

vector<vector<string> > parse(string &cmd, vector<int> list){
    	vector<string> tk;
    	for (unsigned i = 0; i < cmd.size(); i++){
       		if (cmd[i] == ';') list.push_back(0);
        	else if (cmd[i] == '|'){
            		if (cmd[i+1] == '|') {
                		i++;
                		list.push_back(2);
            		}
            		else
                		list.push_back(3);
        	}
        	else if ((cmd[i] == '&') && (cmd[i+1] == '&')){
            		i++;
            		list.push_back(1);
        	}
        	else if (cmd[i] == '<') list.push_back(4);
        	else if (cmd[i] == '>'){
            		if (cmd[i+1] == '>'){
                		i++;
                		list.push_back(6);
            	}
            	else list.push_back(5);
        	}
    }
	vector<vector<string> > vk2;
	vector<string> temp; 
    	tokenizer tokens(cmd, conn);
    	for (tokenizer::iterator z = tokens.begin(); z != tokens.end(); z++){ tk.push_back(*z); }
    	for (unsigned j = 0; j < tk.size(); ++j){
       		temp.clear();
        	tokenizer spced(tk[j], space);
        	for (tokenizer::iterator x = spced.begin(); x != spced.end(); x++){
            		temp.push_back(*x);
        	}
        	vk2.push_back(temp);
    	}
    	return vk2;
}


void execute(vector<vector<string> > &vec, vector<int> &list){
        int con = 0;
	int cs = 0;
	int pick = 0;
    
    	for(unsigned i = 0; i < vec.size(); i++){
        	if (i < list.size())
                con = list.at(i);
        	else con = 0;
        	if (con == 3 || con == 4 || con == 5 || con == 6) i+= pick;
        		vector<char *> argument(vec[i].size() + 1);
        		for(unsigned j = 0; j < vec[i].size(); ++j){
        	    		argument[j] = &vec[i][j][0];
        		}
        	int pid = fork();
        	switch(pid){
            	  case -1:
                	perror("fork");
            	  	exit(1);
		  case 0:
                	if(execvp(vec[i][0].c_str(), argument.data()) == -1){
                    		perror("execvp");
                	}
            	  default:
                	if(wait(&cs) == -1){
                    		perror("wait");
                	}
                	if((cs != 0 && con == 1) || (cs == 0 && con == 2)) return;
        }
    }
}




int main(){
	char *user = getlogin();
	char host[100]; 
 	gethostname(host,sizeof host);
	while(1){
        	string cmd;
       		vector<vector<string> > final;
       		std :: cout << user << "@" << host << "$ ";
        	getline(cin,cmd);
        	if(cmd == "exit") { return 0;}
        	if(!cmd.empty()){
            		cmd.erase(find(cmd.begin(), cmd.end(), '#'), cmd.end());
            		vector<int> p;
            		vector<vector<string> >final = parse(cmd,p);
            		execute(final,p);
		}
	}
	return 0;
}




/*#include <iostream>
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

void com(char* b, char*arg[]){
	int counter = 0;
	char d[] = " \t\n";
	char *ck = strtok(b,d);
	char * temp = NULL;
	while(ck != temp){
		arg[counter++] = ck;
		//counter ++;
		//ck++;
		ck = strtok(temp,d);
	}
	arg[counter] = temp;
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
		if(TRCK1 > 0){
         	       if(close(TRCK1) == -1)
               	       		perror("close");
        	}
        	if(TRCK2 > 0){
                	if(close(TRCK2) == -1)
                        	perror("close");
        	}
		CTR1 = 0;
		CTR2 = 0;
		TRCK1 = 0;
		TRCK2 = 0;
		
	}
	return 0;
}





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
