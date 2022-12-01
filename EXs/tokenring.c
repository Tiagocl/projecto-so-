#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include <time.h>

int main (int argc, char *argv[]) {
    int n = atoi(argv[1]);
    float p = atof(argv[2]);
    int t = atoi(argv[3]);
    int token = 0;
    int prob = 1 / p;
    pid_t pid[n];
    char *writePipe = (char*)malloc(40);
    char *readPipe = (char*)malloc(40);

    char *pipe = (char*)malloc(40);

    for(int i = 1; i <= n; i++) {
        if(i == n)
            sprintf(pipe, "pipe%dto1", i);
        else
            sprintf(pipe, "pipe%dto%d", i, i+1);

        if((mkfifo(pipe, 0666)) < 0) {
            printf ("mkfifo error");
            exit(EXIT_FAILURE);
        }
    }
    free(pipe);

    

    for(int i = 1; i <= n; i++) {

        if((pid[i-1] = fork()) < 0) {
            fprintf(stderr, "%s: fork error: %s\n", argv[0], strerror(errno));
            exit(EXIT_FAILURE);
        } 
        else if(pid[i-1] == 0) {
            if(i == atoi(argv[1])) {
                sprintf(writePipe, "pipe%dto1", i);
                sprintf(readPipe, "pipe%dto%d", i-1, i);
            } else if(i == 1) {
                sprintf(writePipe, "pipe%dto%d", i, i+1);
                sprintf(readPipe, "pipe%dto1", n);
            } else {
                sprintf(writePipe, "pipe%dto%d", i, i+1);
                sprintf(readPipe, "pipe%dto%d", i-1, i);
            }

            srandom(time(NULL) - i * 2);

            //save pipe in the array 
            int fd[2];

            if(i == 1) {
                if((fd[1] = open(writePipe, O_WRONLY)) < 0) {
                    fprintf(stderr, "%s: pipe opening error: %s\n", argv[0], strerror(errno));
                    exit(EXIT_FAILURE);
                }

                token++;

                if(write(fd[1], &token, sizeof(int)) < 0) {
                    fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
                    exit(EXIT_FAILURE);
                }

                close(fd[1]);
            }

            while(1) {

                /* read value from previous process */
                if((fd[0] = open(readPipe, O_RDONLY)) < 0) {
                    fprintf(stderr, "%s: pipe opening error: %s\n", argv[0], strerror(errno));
                    exit(EXIT_FAILURE);
                }

                if(read(fd[0], &token, sizeof(int)) < 0) {
                    fprintf(stderr, "%s: read error: %s\n", argv[0], strerror(errno));
                    exit(EXIT_FAILURE);
                }

                close(fd[0]);

                token++; 
                
                int rand = random() % prob;
                if(rand == 1) {
                    printf("[p%d] lock on token (val = %d)\n", i, token);
                    sleep(atoi(argv[3]));
                    printf("[p%d] unlock token\n", i);
                }

                /* writes value to next process */
                if((fd[1] = open(writePipe, O_WRONLY)) < 0) {
                    fprintf(stderr, "%s: pipe opening error: %s\n", argv[0], strerror(errno));
                    exit(EXIT_FAILURE);
                }
                
                if(write(fd[1], &token, sizeof(int)) < 0) {
                    fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
                    exit(EXIT_FAILURE);
                }

                close(fd[1]);
            }

            exit(EXIT_SUCCESS);
        }
    }

    for(int i = 0; i < atoi(argv[1]); i++) {
        if(waitpid(pid[i], NULL, 0) < 0) {
            fprintf(stderr, "%s: waitpid error: %s\n", argv[0], strerror(errno));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
