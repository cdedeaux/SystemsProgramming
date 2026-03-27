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
    double scl = (rng + 0.0) / (RAND_MAX + 1.0);
    double val;

    if (!seeded){
        seeded = 1;
        srand(time(NULL) + getpid());
    }
    val = rand() * scl + first;
    return (int)val;
}

pthread_mutex_t mutex[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

pthread_cond_t cond[3] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};


int scoreplayer1 = 0;
int scoreplayer2 = 0;
int guess[2];
int cmp[2];
int rdy[4];

void* refereeFunc(void *arg){
    for (int i = 0; i < 10; i++){
        checkThread(pthread_mutex_lock(&mutex[2]), "lock referee");
        int target = genRnd(1, 100);
        rdy[2] = 1;
        rdy[3] = 1;
        checkThread(pthread_cond_broadcast(&cond[2]), "broadcast cond 3");
        checkThread(pthread_mutex_unlock(&mutex[2]), "unlocking referee");
        printf("Round %d: ", i + 1);
        while (1){
            sleep(1);
            checkThread(pthread_mutex_lock(&mutex[0]), "lock player 1");
            checkThread(pthread_mutex_lock(&mutex[1]), "lock player 2");

            cmp[0] = guess[0] - target;
            cmp[1] = guess[1] - target;
            rdy[0] = 1;
            rdy[1] = 1;
            checkThread(pthread_cond_broadcast(&cond[0]), "broadcast cond 1");
            checkThread(pthread_cond_broadcast(&cond[1]), "broadcast cond 2");
            checkThread(pthread_mutex_unlock(&mutex[0]), "unlocking player 1");
            checkThread(pthread_mutex_unlock(&mutex[1]), "unlocking player 2");
            if (cmp[0] == 0 && cmp[1] == 0){
                printf("Draw! Both players guessed %d\n", target);
                break;
            }
            else if (cmp[0] == 0){
                scoreplayer1++;
                printf("Player 1 wins! Guessed %d\n", target);
                break;
            }
            else if (cmp[1] == 0){
                scoreplayer2++;
                printf("Player 2 wins! Guessed %d\n", target);
                break;
            }
        }
    }
    return NULL;
}
void *player1Func(void *arg){
    int id = 0;
    while(1){
        int min = 0, max = 100;
        checkThread(pthread_mutex_lock(&mutex[2]), "lock referee");
        while (rdy[2] == 0){
            checkThread(pthread_cond_wait(&cond[2], &mutex[2]), "wait cond 3");
        }
        rdy[2] = 0;
        checkThread(pthread_mutex_unlock(&mutex[2]), "unlocking referee");
        while (1){
            int guessVal = (max + min) / 2;
            guess[0] = guessVal;
            checkThread(pthread_mutex_lock(&mutex[0]), "lock player 1");
            while(rdy[0] == 0){
                checkThread(pthread_cond_wait(&cond[0], &mutex[0]), "wait cond 1");
            }
            rdy[0] = 0;
            checkThread(pthread_mutex_unlock(&mutex[0]), "unlocking player 1");
            if (cmp[0] == 0){
                break;
            }
            else if (cmp[0] < 0){
                min = guessVal;
            }
            else {
                max = guessVal;
            }
        }
    }
    return NULL;
}

void *player2Func(void *arg){
    while(1){
        int min = 0, max = 100;
        checkThread(pthread_mutex_lock(&mutex[2]), "lock referee");
        while (rdy[3] == 0){
            checkThread(pthread_cond_wait(&cond[2], &mutex[2]), "wait cond 3");
        }
        rdy[3] = 0;
        checkThread(pthread_mutex_unlock(&mutex[2]), "unlocking referee");
        while (1){
            int guessVal = genRnd(min, max);
            guess[1] = guessVal;
            checkThread(pthread_mutex_lock(&mutex[1]), "lock player 2");
            while(rdy[1] == 0){
                checkThread(pthread_cond_wait(&cond[1], &mutex[1]), "wait cond 2");
            }
            rdy[1] = 0;
            checkThread(pthread_mutex_unlock(&mutex[1]), "unlocking player 2");
            if (cmp[1] == 0){
                break;
            }
            else if (cmp[1] < 0){
                min = guessVal;
            }
            else {
                max = guessVal;
            }
        }
    }
    return NULL;
}

int main(){
    rdy[1] = 0;
    rdy[2] = 0;
    rdy[3] = 0;
    rdy[0] = 0;
    scoreplayer1 = 0;
    scoreplayer2 = 0;
    pthread_t refThread, p1Thread, p2Thread;
    pthread_create(&refThread, NULL, refereeFunc, NULL);
    pthread_create(&p1Thread, NULL, player1Func, NULL);
    pthread_create(&p2Thread, NULL, player2Func, NULL);
    pthread_join(refThread, NULL);
    printf("Final Score: Player 1: %d, Player 2: %d\n", scoreplayer1, scoreplayer2);
    return 0;
}
