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
    int val;
    char filename[50] = "integer.dat";
    ssize_t numRd;

    fd = checkError(open(filename, O_RDONLY), "file open failed");

    for (i = 0; i < 100; i++){
        numRd = checkError(read(fd, &val, sizeof(int)), "Failed to read integer");
        printf("%d ",val);
    }
    printf("\n\n");
    numRd = checkError(read(fd, arr, 100 * sizeof(int)), "Failed to read array");

    for (i = 0; i < numRd/sizeof(int); i++){
        printf("%d ", arr[i]);
    }
    printf("\n\n");
    close(fd);
    return 0;
}