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

int got_usr1 = 0;
int got_usr2 = 0;
int got_hup = 0;

int game;

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

int getTotal(){
    int file = checkError(open("total.txt", O_RDONLY), "open total.txt");
    int total;
    read(file, &total, sizeof(int));
    close(file);
    return total;
}

void makeTotal(int total){
    int file = checkError(open("total.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "open total.txt");
    write(file, &total, sizeof(int));
    close(file);
}

void siguser1Handler(int sig){
    got_usr1 = 1;
}

void siguser2Handler(int sig){
    got_usr2 = 1;
}

void sighupHandler(int sig){
    got_hup = 1;
}

int main(){
    struct sigaction sa1 = {0};
    sa1.sa_handler = siguser1Handler;
    sigaction(SIGUSR1, &sa1, NULL);

    struct sigaction sa2 = {0};
    sa2.sa_handler = siguser2Handler;
    sigaction(SIGUSR2, &sa2, NULL);

    struct sigaction sa3 = {0};
    sa3.sa_handler = sighupHandler;
    sigaction(SIGHUP, &sa3, NULL);

    pid_t child_pid1 = fork();
    int i;
    if (child_pid1 == 0){
        for (i = 0; i < 10; i++){
            kill(getppid(), SIGUSR1);

            while(!got_usr1 && !got_usr2){
                pause();
            }
            int is_21 = got_usr1;
            got_usr1 = 0;
            got_usr2 = 0;

            kill(getppid(), SIGUSR1);

            while(1){
                while(!got_usr1 && !got_usr2){
                    pause();
                }
                if(got_usr2){
                    got_usr2 = 0;
                    break;
                }
                got_usr1 = 0;

                int total = getTotal();
                int temp = 0;

                if(is_21){
                    temp = genRnd(1, 3);
                } 
                else {
                    int targets[] = {1, 12, 23, 34, 45, 56, 67, 78, 89, 100};
                    for(int g = 0; g < 10; g++){
                        if(targets[g] > total){
                            temp = targets[g] - total;
                            break;
                        }
                    }
                    if(temp > 10){
                        temp = 1;
                    }
                    if(temp <= 0){
                        temp = 1;
                    }
                }

                total += temp;
                makeTotal(total);
                printf("Player 1 -- Adds %d to make the total %d\n", temp, total);
                
                kill(getppid(), SIGUSR1);
            }
        }
        exit(0);
    }

    pid_t child_pid2 = fork();
    if (child_pid2 == 0){
        for (i = 0; i < 10; i++){
            
            kill(getppid(), SIGUSR2);

            while(!got_hup && !got_usr2){
                pause();
            }
            int is_21;
            if(got_usr2){
                got_usr2 = 0;
                is_21 = 0;
            }
            else{
                got_hup = 0;
                is_21 = 1;
            }

            kill(getppid(), SIGUSR2);

            while(1){
                while(!got_hup && !got_usr2){
                    pause();
                }
                if(got_usr2){
                    got_usr2 = 0;
                    break;
                }
                got_hup = 0;

                int total = getTotal();
                int temp = 0;

                if(is_21){
                    temp = 4 - total % 4;
                    if(temp == 4){
                        temp = genRnd(1, 3);
                    }
                } 
                else {
                    temp = genRnd(1, 10);
                    if(total + temp > 100){
                        temp = 100 - total;
                    }
                }

                total += temp;
                makeTotal(total);
                printf("Player 2 -- Adds %d to make the total %d\n", temp, total);
                
                kill(getppid(), SIGUSR2);
            }
        }
        exit(0);
    }

    int score1 = 0;
    int score2 = 0;

    for(i = 0; i < 10; i++){
        
        while(!got_usr1){
            pause();
        }
        got_usr1 = 0;

        while(!got_usr2){
            pause();
        }
        got_usr2 = 0;

        game = genRnd(0, 1);
        makeTotal(0);

        if(game == 1){
            printf("Round %d: Playing 21:\n" , i +1);
            kill(child_pid1, SIGUSR1);
            kill(child_pid2, SIGHUP);
        }
        else{
            printf("Round %d: Playing 100:\n" , i +1);
            kill(child_pid1, SIGUSR2);
            kill(child_pid2, SIGUSR2);
        }

        while(!got_usr1){
            pause();
        }
        got_usr1 = 0;

        while(!got_usr2){
            pause();
        }
        got_usr2 = 0;

        int winner = 0;
        while(!winner){
            kill(child_pid1, SIGUSR1);
            while(!got_usr1){
                pause();
            }
            got_usr1 = 0;

            int total = getTotal();
            if(game == 1 && total >= 21){
                winner = 2;
            }
            else if(game == 0 && total >= 100){
                winner = 1;
            }

            if(!winner){
                kill(child_pid2, SIGHUP);
                while(!got_usr2){
                    pause();
                }
                got_usr2 = 0;

                total = getTotal();
                if(game == 1 && total >= 21){
                    winner = 1;
                }
                else if(game == 0 && total >= 100){
                    winner = 2;
                }
            }
        }

        if(winner == 1){
            score1++;
        }
        else{
            score2++;
        }
        printf("Player %d wins Score -- Player 1: %d | Player 2: %d\n", winner, score1, score2);

        kill(child_pid1, SIGUSR2);
        kill(child_pid2, SIGUSR2);
    }

    wait(NULL);
    wait(NULL);
    return 0;
}