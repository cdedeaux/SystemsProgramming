#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main(int argc, char *argv[]){
    int fd;
    int i;
    int arr[100];
    ssize_t numWr;
    char filename[50] = "integer.dat";

    fd = checkError(open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "file open failed");
    //fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    //instead of S_IRSUSR | S_IWUS R, we can use 0600
    // Levels of permission :     U        G       O
    //                           rwx      rwx     rwx
    //                           110      000     000
    //                            6        0       0
    //those are triples so octal
    //octal number represented with leading 0

    for(i = 0; i < 100; i++){
        arr[i] = i + 1;
    }

    for (i = 0; i < 100; i++){
        numWr = checkError(write(fd, &arr[i], sizeof(int)), "Failed to write in");
    }

    numWr = checkError(write(fd, arr, 100 * sizeof(int)), "Failed to write array");

    close(fd);
    return 0;
}

//