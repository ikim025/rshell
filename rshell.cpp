#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]){
    string cmd;
    while(cmd != "exit"){
        std::cout << "$ ";
        cin >> cmd;
    }
    return 0;
}
