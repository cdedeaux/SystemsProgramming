#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
int main (){
    struct dirent *d;
    DIR *dirp;
    struct stat s;
    time_t t;

    if (stat("./main.c", &s) == -1){
        perror("stat");
        return 1;
    }
    t = s.st_mtime;
    c = ctime(&t);
    printf("%s", c);
    if ((dirp = opendir(".")) == NULL){
        perror("opendir");
        return 1;
    }

    while ((d = readdir(dirp)) != NULL){
        printf("%s\n", d->d_name);
    }
    closedir(dirp);
    return 0;
}