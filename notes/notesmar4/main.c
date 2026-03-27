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
int genRnd(int first, int last){
    int rng = last - first + 1;
    static int seeded;
    double scl = (rng + 0.0) / INT_MAX;

    if (!seeded){
        seeded = 1;
        srand(time(NULL) + getpid());
    }
    val = rand() * scl + first;
    return val;
}
pthread_mutex_t MTX = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CND = PTHREAD_COND_INITIALIZER;

int data[20];


pthread_mutex_t TIMING = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t TIMING_CND = PTHREAD_COND_INITIALIZER;

int go;


void *startProducer(void *arg){
    int cnt = 0;
    int loop = 0;
    values = genRnd(5, 20);
    for (loop = 0; loop < 5; loop++){
        
            checkThread(pthread_mutex_lock(&MTX), "pthread_mutex_lock");
        for (cnt = 0; cnt < values; cnt++){
            data[cnt] = genRnd(0, 1000);
        }
        checkThread(pthread_mutex_unlock(&MTX), "pthread_mutex_unlock");
        checkThread(pthread_cond_signal(&CND), "pthread_cond_signal");


        checkThread(pthread_mutex_lock(&TIMING), "pthread_mutex_lock");
        while (!go){
            checkThread(pthread_cond_wait(&TIMING_CND, &TIMING), "pthread_cond_wait");
        }
        checkThread(pthread_mutex_unlock(&TIMING), "pthread_mutex_unlock");
    }
    pthread_exit(NULL);

}

void *startConsumer(void *arg){
    int sum = 0;
    double avg = 0;
    int loop = 0;
    int cnt = 0;
    int values = 0;
    for (loop = 0; loop < 5; loop++){
        checkThread(pthread_mutex_lock(&TIMING), "pthread_mutex_lock");
        checkThread(pthread_mutex_lock(&MTX), "pthread_mutex_lock");
        while (values < 1){
            checkThread(pthread_cond_wait(&CND, &MTX), "pthread_cond_wait");
        }
        printf("values: ");
        for (cnt = 0; cnt < values; cnt++){
            printf("%d ", data[cnt]);
            sum += data[cnt];
        }
        avg = (sum + 0.0) / values;

        values = 0;
        checkThread(pthread_mutex_unlock(&MTX), "pthread_mutex_unlock");
        printf("\nAverage = %lf\n", avg);
        checkThread(pthread_mutex_unlock(&TIMING), "pthread_mutex_unlock");
        
        pthread_exit(NULL);
    }
}

int main(int argc, char *argv[]){
    pthreat_t thrs[2];
    void *(*fcns[2])(void *) = {startProducere, startConsumer};

    int i = 0;
    for (i = 0; i < 2; i++){
        checkThread(pthread_create(&thrs[i], NULL, fcns[i], NULL), "pthread_create");
    }
    for (i = 0; i < 2; i++){
        checkThread(pthread_join(thrs[i], NULL), "pthread_join");
    }

    
    return 0;
}