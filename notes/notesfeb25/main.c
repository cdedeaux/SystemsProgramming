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

int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int checkThread(int val, const char *msg){
    if (val > 0){
        errno = val;
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
struct thrRet{
    int i;

};
struct thrArgs{
    int i;

};

void *startThread(void *args){
    struct timespec ts = {5, 0};
    struct thrArgs *args = (struct thrArgs *)args;    
    struct thrRet *ptr = (struct thrRet *) malloc(sizeof(struct thrRet));
    
    ptr->i = args->i;
    printf("Starting thread %d\n", args->i);
    nanosleep(&ts, NULL);
    printf("Ending thread %d\n", args->i);

    //return NULL;
    pthread_exit(NULL);
}

int main()[
    pthread_t thr[3];
    int i = 0;

    struct thrArgs args[3];
    void *ret;
    int i = 0;
    int *iptr;
    struct thrRet *rptr;
    for (i = 0; i < 3; i++){
        args[i].i = i;
        checkThread(pthread_create(&thr[i], NULL, startThread, &args[i]), "pthread_create");
    }

    for (i = 0; i < 3; i++){
        checkThread(pthread_join(thr[i], &ret), "pthread_join");
        rptr = (struct thrRet *)ret;
        printf("Thread returned %d\n",rptr->i);
        free(rptr);
    }


    return 0;
]