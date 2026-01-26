#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
int main(){
    const char *filepath = "raw.dat";
    const char *dataout = "data.dat";
    int fd = checkError(open(filepath, O_RDONLY), "Failed to open input file");    int fd_out = checkError(open(dataout, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Failed to open output file");
    unsigned char buffer[20];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, 20)) > 0) {
        short accaax = (short)(buffer[3] << 8 | buffer[2]);
        short accay = (short)(buffer[5] << 8 | buffer[4]);
        short accaaz = (short)(buffer[7] << 8 | buffer[6]);

        short angwx = (short)(buffer[9] << 8 | buffer[8]);
        short angwy = (short)(buffer[11] << 8 | buffer[10]);
        short angwz = (short)(buffer[13] << 8 | buffer[12]);

        short rolldat = (short)((buffer[15] << 8 | buffer[14]) );
        short pitchdat = (short)((buffer[17] << 8 | buffer[16]));
        short yawdat = (short)((buffer[19] << 8 | buffer[18]));

        float ax = (accaax / 32768.0f) * 16.0f;
        float ay = (accay / 32768.0f) * 16.0f;
        float az = (accaaz / 32768.0f) * 16.0f;

        float wx = (angwx / 32768.0f) * 2000.0f;
        float wy = (angwy / 32768.0f) * 2000.0f;
        float wz = (angwz / 32768.0f) * 2000.0f;

        float roll = ((float)rolldat/32768.0f) * 180.0f;
        float pitch = ((float)pitchdat/32768.0f) * 180.0f;
        float yaw = ((float)yawdat/32768.0f) * 180.0f;

        write(fd_out, &ax, sizeof(float));
        write(fd_out, &ay, sizeof(float));
        write(fd_out, &az, sizeof(float));
        write(fd_out, &wx, sizeof(float));
        write(fd_out, &wy, sizeof(float));
        write(fd_out, &wz, sizeof(float));
        write(fd_out, &roll, sizeof(float));
        write(fd_out, &pitch, sizeof(float));
        write(fd_out, &yaw, sizeof(float));
        printf("ax=%.2f, ay=%.2f, az=%.2f, wx=%.2f, wy=%.2f, wz=%.2f, roll=%.2f, pitch=%.2f, yaw=%.2f\n",
               ax, ay, az, wx, wy, wz, roll, pitch, yaw);

    }
    close(fd);
    return 0;
}