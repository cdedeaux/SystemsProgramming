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

int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
void signalHandler(int sig){
    struct itimerval zeroTimer = {0};
    setitimer(ITIMER_REAL, &zeroTimer, NULL);

    printf("\nExit (y/n)?\n");
    char response;
    read(STDIN_FILENO, &response, 1);
    if(response == 'y' || response == 'Y'){
        exit(EXIT_SUCCESS);
    } else {
        printf("Continuing.\n");
    }
}
void childHandler(int sig){
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0);

    if (pid == -1) {
        write(STDOUT_FILENO, "No more children to wait for.\n", 31);
        _exit(0);
    }
}

void siguser1Handler(int sig){
    write(STDOUT_FILENO, "Warning! roll outside of bounds\n", 32);
}

void siguser2Handler(int sig){
    write(STDOUT_FILENO, "Warning! pitch outside of bounds\n", 33);
}

void sigtermHandler(int sig){
    _exit(0);
}
int main(){
    struct timespec timer; 
    timer.tv_sec = 5;
    timer.tv_nsec = 0;

    signal(SIGINT, signalHandler);
    signal(SIGCHLD, childHandler);
    signal(SIGUSR1, siguser1Handler);
    signal(SIGUSR2, siguser2Handler);
    write(STDOUT_FILENO, "Setting up signals\n", 19);
    nanosleep(&timer, NULL);
    pid_t child_pid = fork();
    if(child_pid == 0){
        struct timespec req;
        req.tv_sec = 1;
        req.tv_nsec = 0;
        const char *filepath = "angl.dat";
        int fd = checkError(open(filepath, O_RDONLY), "open");
        ssize_t bytesRead;
        double low = -20.0, high = 20.0;
        double roll;
        double pitch;
        double yaw;

        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        signal(SIGTERM, sigtermHandler);

        while ((bytesRead = read(fd, &roll, sizeof(double))) > 0){
            checkError(bytesRead, "read roll");
            bytesRead = read(fd, &pitch, sizeof(double));
            checkError(bytesRead, "read pitch");
            bytesRead = read(fd, &yaw, sizeof(double));
            checkError(bytesRead, "read yaw");
            
            if(roll < low || roll > high){
                kill(getppid(), SIGUSR1);
            }
            if(pitch < low || pitch > high){
                kill(getppid(), SIGUSR2);
            }
            
            
            printf("Values within bounds: Roll: %.2f Pitch: %.2f Yaw: %.2f\n", roll, pitch, yaw);
            nanosleep(&req, NULL);

        }
        close(fd);
        _exit(0);
            
        
    }
    else{
        write(STDOUT_FILENO, "Parent process waiting for signal\n", 34);
        while(1){
            pause();
        }
    }
    

    return 0;
}
//sorry this is ugly, i copied and pasted bad the pieces bad from my other programs