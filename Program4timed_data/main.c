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

int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}


int main(){
    const char *filepath = "angl.dat";
    int fd = checkError(open(filepath, O_RDONLY), "open");
    ssize_t bytesRead;
    struct timespec req;
    req.tv_sec = 1;
    req.tv_nsec = 0;


    double low = -20.0, high = 20.0;
    double roll;
    double pitch;
    double yaw;
    while ((bytesRead = read(fd, &roll, sizeof(double))) > 0){
        checkError(bytesRead, "read roll");
        bytesRead = read(fd, &pitch, sizeof(double));
        checkError(bytesRead, "read pitch");
        bytesRead = read(fd, &yaw, sizeof(double));
        checkError(bytesRead, "read yaw");
        if(roll < low || roll > high || pitch < low || pitch > high){
            printf("ALERT Values not in bounds: Roll: %.2f Pitch: %.2f Yaw: %.2f\n", roll, pitch, yaw);
        }
        else{
            printf("Values within bounds: Roll: %.2f Pitch: %.2f Yaw: %.2f\n", roll, pitch, yaw);
        }
        nanosleep(&req, NULL);
    }
    close(fd);

    return 0;
}