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
    const char *filepath = "data.dat";
    int fd = checkError(open(filepath, O_RDONLY), "open");
    ssize_t bytesRead;
    
    mkdir("values", 0755);
    
    int fd_accl = checkError(open("values/accl.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "open accl.dat");
    int fd_rota = checkError(open("values/rota.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "open rota.dat");
    int fd_angl = checkError(open("values/angl.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "open angl.dat");

    float ax, ay, az;
    float wx, wy, wz;
    float roll, pitch, yaw;

    while ((bytesRead = read(fd, &ax, sizeof(float))) > 0) {
        read(fd, &ay, sizeof(float));
        read(fd, &az, sizeof(float));
        read(fd, &wx, sizeof(float));
        read(fd, &wy, sizeof(float));
        read(fd, &wz, sizeof(float));
        read(fd, &roll, sizeof(float));
        read(fd, &pitch, sizeof(float));
        read(fd, &yaw, sizeof(float));

        write(fd_accl, &ax, sizeof(float));
        write(fd_accl, &ay, sizeof(float));
        write(fd_accl, &az, sizeof(float));

        write(fd_rota, &wx, sizeof(float));
        write(fd_rota, &wy, sizeof(float));
        write(fd_rota, &wz, sizeof(float));

        write(fd_angl, &roll, sizeof(float));
        write(fd_angl, &pitch, sizeof(float));
        write(fd_angl, &yaw, sizeof(float));
    }
    
    close(fd);
    close(fd_accl);
    close(fd_rota);
    close(fd_angl);

    return 0;
}