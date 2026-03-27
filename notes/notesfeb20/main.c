#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
//example of exec
int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
void signalHandler(int sig){
    pid_t p;
    if(sig == SIGCHLD){
        while((p = waitpid(0, NULL, WNOHANG)) > 0){
            if (p == -1){
                if (errno == ECHILD){
                    exit(EXIT_SUCCESS);
                }
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        }
    }
}
int main(){
    struct sigaction sa;
    pid_t p;
    char *args[] = {"/usr/bin/ls", "-l", "-a", NULL}; //c vector of args for exec

    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    checkError(sigaction(SIGCHLD, &sa, NULL), "sigaction");
    p = checkError(fork(), "fork");

    if(p == 0){
        //child
        //execlp users shells current path, execl needs full path, execv takes vector of args
        execl("/usr/bin/ls", "/usr/bin/ls" , "-l", "-a", NULL);
        execv(args[0], args);
        printf("This should never print\n");
        exit(EXIT_SUCCESS);
    }
    //in parent
    while(1){
        pause();
    }
    return 0;
}