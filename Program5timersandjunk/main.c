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
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
void cleanIO()
{
    printf("Press Enter to continue...\n");
  char garbage[100];
  static int once = 0;
  static struct termios old_tio, new_tio;
  if (!once)
    {
      tcgetattr(STDIN_FILENO, &old_tio);
      new_tio = old_tio;
      cfmakeraw(&new_tio);
      once = 1;
    }

  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  read(STDIN_FILENO, garbage, 100);

  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

int checkError(int val, const char *msg){
    if(val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}
void signalHandler(int signum){
    if(signum == SIGALRM){
    }
}
void signalHandler2(int signum){
    struct itimerval zeroTimer = {0};
    setitimer(ITIMER_REAL, &zeroTimer, NULL);

    printf("\nExit (y/n)?\n");
    char response;
    read(STDIN_FILENO, &response, 1);
    if(response == 'y' || response == 'Y'){
        printf("Exiting quiz.\n");
        exit(EXIT_SUCCESS);
    } else {
        printf("Continuing quiz.\n");
    }
}

int main(){
    const char *filepath = "text.txt";
    int fd = checkError(open(filepath, O_RDONLY), "open");
    char buffer[100];
    ssize_t bytesRead;
    struct itimerval timer;
    struct itimerval remainingTime;
    timer.it_value.tv_sec = 15;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    int totalTime = 0;
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    struct sigaction sa_int;
    sa_int.sa_handler = signalHandler2;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    
    printf("This is a timed typing quiz\nYou will have 15 seconds to type the text\nPress Ctrl+C to exit the quiz\nDo you want to start? (y/n): \n");
    char start;
    read(STDIN_FILENO, &start, 1);
    if (start != 'y' && start != 'Y') {
        printf("Exiting quiz.\n");
        close(fd);
        exit(EXIT_SUCCESS);
    }
    for (int i = 0; i < 3; i++) {
        printf("Get ready... %d\n", 3 - i);
        sleep(1);
    }
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        char currentLine[100];
        int lineIn = 0;
        for (int i = 0; i < bytesRead; i++) {
            if (i==0 || buffer[i-1] == '\n') {
                printf("Type the following line:\n");
            }
            write(1, &buffer[i], 1);
            
            if (buffer[i] != '\n' && buffer[i] != '\r') {
                currentLine[lineIn++] = buffer[i];
            } else {
                currentLine[lineIn] = '\0';
            }
            if (buffer[i] == '\n') {
                if(buffer[i-1] == '\r'){
                    buffer[i-1] = '\0';
                }

                setitimer(ITIMER_REAL, &timer, NULL);
                
                char input[100];
                ssize_t inputBytes = read(0, input, sizeof(input) - 1);
                int savedErrno = errno;
                getitimer(ITIMER_REAL, &remainingTime);

                struct itimerval zeroTimer = {0};

                setitimer(ITIMER_REAL, &zeroTimer, NULL);

                if (inputBytes == -1 && savedErrno == EINTR) {
                    write(STDOUT_FILENO, "Time has elapsed\n", 17);
                    cleanIO();
                } 
                else {
                    int remainingSec = remainingTime.it_value.tv_sec;
                    input[inputBytes] = '\0';

                    if (input[inputBytes - 1] == '\n') {
                        input[inputBytes - 1] = '\0';
                    }
                    if (strcmp(input, currentLine) == 0) {
                        printf("Correct! You typed the line correctly. (%d seconds remaining)\n", remainingSec);
                        totalTime += (remainingSec);
                    } 
                    else {
                        printf("Incorrect. The correct line was: %s\n", currentLine);
                    }
                }
                lineIn = 0;
            }
        }
        printf("Total score : %d \n", totalTime);
    }


    return 0;
}
