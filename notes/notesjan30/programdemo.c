#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
//make file called y.dat that contains ints 1-x
// where y is the filename specified by the user
// and x is an integer sepcified by the user
// x should be the 1st command line arg
// y should be the 2nd command line arg
int main(int argc, char *argv[]){
    int i = 0;
    int max = 0;
    char filename[50];
    char *ext = ".dat";
    int fd;
    ssize_t numWr;

    if (argc < 3){
        printf("syntax: %s max filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sscanf(argv[1], "%d", &max);
    //strncpy(filename, argv[2], 50);
    //strncat(filename, ext, 50;
    //printf("filename = %s\n", filename);

    sprintf(filename , "%s.dat", argv[2]);
    printf("filename = %s\n", filename);
    fd = checkError(open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600), "open");
    for (i = 0; i <= max; i++){
        int val = i +1;
        numWr = checkError(write(fd, &val, sizeof(int)), "write");
    }
    for (i = 0; i < argc; i++){
        printf("arg %d = %s\n", i, argv[i]);
    }
    


    
    return 0;
}