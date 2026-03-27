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




int main(){

    return 0;
}
