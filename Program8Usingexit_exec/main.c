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
    double val;

    if (!seeded){
        seeded = 1;
        srand(time(NULL) + getpid());
    }
    val = rand() * scl + first;
    return (int)val;
}

int main(int argc, char *argv[]){

    pid_t pid = fork();
    checkError(pid, "fork");
    int exitCode = 0;
    int sum = 0;
    if (pid == 0){
        execl("./myRand", "./myRand", NULL);
        checkError(-1, "execl");
        exit(EXIT_FAILURE);
    }
    else{
        int status;
        checkError(wait(&status), "wait");
        exitCode = WEXITSTATUS(status);
    }
    char filename[32];
        sprintf(filename, "data%d.dat", exitCode);

    int file = checkError(open(filename, O_RDONLY, 0644), "open dataX.dat");

    for (int i = 0; i < 60; ++i) {
        int value;
        checkError(read(file, &value, sizeof(value)), "read value");
        printf("%d ", value);
        sum += value;
        printf("\n");
        sleep(1); // added to make look better
    }
    printf("Average: %.2f\n", sum / 60.0);
    checkError(unlink(filename), "unlink dataX.dat");
    close(file);
    return 0;


}