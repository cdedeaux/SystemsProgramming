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


void signalHandler(int sig){
    if (sig == SIGALRM) {
        printf("Time's up!\n");
    }
}


int main(){
    struct sigaction sa;
    int x;
    struct itimerval it = {1,0,1,0};

    sa.sa_handler = signalHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGALRM, &sa, NULL);

    setitimer(ITIMER_REAL, &it, NULL);

    while (1)

    {

        scanf("%d",&x);

        printf("tic\n");

    }
}

