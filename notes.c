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
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <limits.h>

int got_usr1 = 0;
int got_usr2 = 0;
int quit = 0;

void siguser1Handler(int sig){
    got_usr1 = 1;
}

void siguser2Handler(int sig){
    got_usr2 = 1;
}

void sigintHandler(int sig){
    exit(0);
}

void sigtermHandler(int sig){
    quit = 1;
}

int main(){
    
    struct sigaction sa = {0};
    
    sa.sa_handler = signalHandler;
    
    sigemptyset(&sa.sa_mask);
    
    sigaction(SIGUSR1, &sa, NULL);
    
    sigaction(SIGUSR2, &sa, NULL);
    
    sigaction(SIGINT, &sa, NULL);
    
    sigaction(SIGTERM, &sa, NULL);
    return 0;
}
