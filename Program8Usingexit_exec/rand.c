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

int main(){
    int X = genRnd(0, 255);
    char filename[32];
    sprintf(filename, "data%d.dat", X);


    int file = checkError(open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644), "open dataX.dat");
    for (int i = 0; i < 60; ++i) {
        int value = genRnd(0, 100);
        checkError(write(file, &value, sizeof(value)), "write value");
    }

    close(file);
    exit(X);
}