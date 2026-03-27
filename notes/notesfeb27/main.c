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

int checkThread(int val, const char *msg){
    if (val > 0){
        errno = val;
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
struct threadArgs{
    int i;
};

int glob;

pthread_mutex_t MTX1 = PTHREAD_MUTEX_INITIALIZER; //locks threads when they want to access the global variable
//critical section is the part of the code where the global variable is accessed, so we need to lock it to prevent race



void * startThread(void *arg){
    struct threadArgs *args = (struct threadArgs *)arg;
    int loc;
    int i = 0;
    for (i = 0; i < args->i; i++){
        checkThread(pthread_mutex_lock(&MTX1), "pthread_mutex_lock");
        glob++;
        checkThread(pthread_mutex_unlock(&MTX1), "pthread_mutex_unlock");
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    struct threadArgs args;
    pthread_t thr[2];
    int i = 0;

    args.i = 50;
    if (argc > 1){
        sscanf(argv[1], "%d", &args.i);
    }

    for (i = 0; i < 2; i++){
        checkThread(pthread_create(&thr[i], NULL, startThread, (void *)&args), "pthread_create");
    }
    for (i = 0; i < 2; i++){
        checkThread(pthread_join(thr[i], NULL), "pthread_join");
    }
    printf("Global value: %d\n", glob);
    return 0;
}