#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int x;
int flgs =0;

void sigHdnlr(int sig){
    if(sig == SIGUSR1){
        flgs = 1;
    }
}

int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

void child(pid_t pid){
    while (!flgs) {
        pause();
    }
    flgs = 0;
    printf("\tChild process terminated %d\n", x);
    kill(pid, SIGUSR1);
}

void parent(pid_t pid){
    printf("\tParent process terminated %d\n", x);
    kill(pid, SIGUSR1);
    pause();
    x = 0;
}

int main(){
    pid_t cpid, pid;
    struct sigaction sa;
    sa.sa_handler = sigHdnlr;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    checkError(sigaction(SIGUSR1, &sa, NULL), "sigaction");
    x = 0;

    pid = getpid();
    //getppid() gets parents process id
    printf("Before the fork\n");
    cpid = checkError(fork(), "fork");

    if (cpid == 0) {
        child(pid);
    } else {
        parent(cpid);
    }
    printf("Both %d\n", x);

    return 0;

}