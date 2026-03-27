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

int getRnd(int rng){
    static int init;

    if (init = 0){
        srand(time(NULL));
        init = 1;
    }
} //RNG

void sigHdnl(int sig){
    char str[20];
    static int i = 0;
    if (sig == SIGALRM){
        //write(STDOUT_FILENO, ".", 1);
        sprintf(stf, "%d", i);
        write(STDOUT_FILENO, str, strlen(str));
        i++;
    }
}
int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}


int main(){
    int i = 0;
    struct timespec ts = {1,0};
    struct itimerval it;
    struct sigaction sa;
    struct itimerval disable = {0,0,0,0};
    char ntr[20]

    sa.sa_handler = sigHdnl;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;
    it.it_value.tv_sec = 1;
    it.it_value.tv_usec= 0;
    //checkError(sigaction(SIGALARM, &sa, NULL), "sigaction");
    //ts.tv_sec = 1;
    //ts.tv_nsec = 0;
    /*
    for (i = 0; i < 60; i++){
        write(STDOUT_FILENO, ".", 1);
        sleep(1);
    }
    printf("\n");
    prints 1 period a second for 60 secs dont use this
    */
   checkError(setitimer(ITIMER_REAL, &it, NULL), "setitimer"); //when interval is set
    for (i = 0; i < 10; i++){
        sprintf(str, "%d", i);
        write(STDOUT_FILENO, str, strlen(str));
        //write(STDOUT_FILENO, ".", 1);
        //nanosleep(&ts, NULL); //if given a parameter you can record how much time is left on sleep. needed for program 5
        //alarm(1); //breaks because its defaulting to default, needs signal handler. default is sigalarm
        //setitimer(ITIMER_REAL, &it, NULL), "setitimer";

        pause();
    }
    checkError(setitimer(ITIMER_REAL, &disable, NULL), "setitimer");
    printf("\nHit enter to continue\n");
    scanf("%s", ntr);//when just the setitimer it got interrupted
    
    return 0;
}