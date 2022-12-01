#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#define MAX_LENGTH 20
#define ARGS_NUMB 4
#define WRITE 1
#define READ 0

int main(int argc, char *argv[])
{
    //Verifies number of arguments
    if (argc < ARGS_NUMB)
    {
        {
            printf("Too few arguments.\n");
            exit(EXIT_FAILURE);
        }

        // Converts process number to int
        int procNumber = atoi(argv[1]);
        

        // Converts probability to float 
        int probability = atof(argv[2]) * 100;
        

        // Converts seconds number to int
        int sec = atoi(argv[3]);
        

        // Creates all pipes
        for (int i = 1; i <= procNumber; i++)
        {
            char *pipe = (char *)malloc(sizeof(char) * MAX_LENGTH);
            // pipe name
            if (i == procNumber)
            {
                sprintf(pipe, "pipe%dto1", i);
            }
            else
            {
                sprintf(pipe, "pipe%dto%d", i, i + 1);
            }
            // Creates fifo with read and write permissions
            if ((mkfifo(pipe, 0666)) < 0)
            {
                fprintf(stderr, "%s: mkfifo error: %s", argv[0], strerror(errno));
                exit(EXIT_FAILURE);
            }
            free(pipe);
        }

        // Initialises token 
        int token = 0;

        // Creates processes
        pid_t pids[procNumber];
        for (int i = 0; i < procNumber; i++)
        {
            char *w_pipe = (char *)malloc(sizeof(char) * MAX_LENGTH);
            char *r_pipe = (char *)malloc(sizeof(char) * MAX_LENGTH);
            if ((pids[i] = fork()) < 0)
            {
                fprintf(stderr, "%s: fork error: %s\n", argv[0], strerror(errno));
                exit(EXIT_FAILURE);
            }
            else if (pids[i] == 0)
            {
                // Reading and writing pipes names
                if (i == 0)
                {
                    sprintf(w_pipe, "pipe1to2");
                    sprintf(r_pipe, "pipe%dto1", procNumber);
                }
                else if (i == procNumber - 1)
                {
                    sprintf(w_pipe, "pipe%dto1", procNumber);
                    sprintf(r_pipe, "pipe%dto%d", procNumber - 1, procNumber);
                }
                else
                {
                    sprintf(w_pipe, "pipe%dto%d", i + 1, i + 2);
                    sprintf(r_pipe, "pipe%dto%d", i, i + 1);
                }

                // Probability generator seed
                srandom(time(NULL) - i);

                int fd[2];

                // If is the first one only writes to the next because dont needs to read
                if (i == 0 && token == 0)
                {
                    // Opens write pipe
                    if ((fd[WRITE] = open(w_pipe, O_WRONLY)) < 0)
                    {
                        fprintf(stderr, "%s: open pipe (%s) error: %s\n", argv[0], w_pipe, strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    token++;

                    // Writes value from token to pipe
                    if (write(fd[WRITE], &token, sizeof(int)) < 0)
                    {
                        fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    close(fd[WRITE]);
                }

                while (1)
                {
                    // Reads previous process value
                    if ((fd[READ] = open(r_pipe, O_RDONLY)) < 0)
                    {
                        fprintf(stderr, "%s: open pipe (%s) error: %s\n", argv[0], r_pipe, strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    if (read(fd[READ], &token, sizeof(int)) < 0)
                    {
                        fprintf(stderr, "%s: read error: %s\n", argv[0], strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    close(fd[READ]);

                    token++;

                    // Probability to lock token
                    int rand = (random() % 100) + 1;
                    if (rand <= probability)
                    {
                        printf("[p%d] lock on token (val = %d)\n", i + 1, token);
                        sleep(sec);
                        printf("[p%d] unlock token\n", i + 1);
                    }

                    // Writes the already incremented token value to next process
                    if ((fd[WRITE] = open(w_pipe, O_WRONLY)) < 0)
                    {
                        fprintf(stderr, "%s: open pipe (%s) error: %s\n", argv[0], w_pipe, strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    if (write(fd[WRITE], &token, sizeof(int)) < 0)
                    {
                        fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    close(fd[WRITE]);
                }

                exit(EXIT_SUCCESS);
            }
            free(w_pipe);
            free(r_pipe);
        }

        // Parent process waits child process to end
        for (int i = 0; i < procNumber; i++)
        {
            if (waitpid(pids[i], NULL, 0) < 0)
            {
                fprintf(stderr, "%s: waitpid (%d) error: %s\n", argv[0], pids[i], strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        exit(EXIT_SUCCESS);
    }
}