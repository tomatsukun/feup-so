#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_STR_SIZE 64
#define N_ARGS 3

// system("touch teste.txt");

const char *WorkInChild(char *txt)
{

    char str_epub[MAX_STR_SIZE];
    char command[MAX_STR_SIZE];
    char zip_command[MAX_STR_SIZE];
    strcpy(str_epub, "");
    strcpy(command, "");
    strcpy(zip_command, "");

    strcpy(str_epub, txt);

    // passa de .txt para .epub
    str_epub[strlen(str_epub) - 3] = 'e';
    str_epub[strlen(str_epub) - 2] = 'p';
    str_epub[strlen(str_epub) - 1] = 'u';
    str_epub[strlen(str_epub) - 0] = 'b';

    // aqui a string ja esta em .epub

    strcat(command, "pandoc ");
    // pando

    strcat(command, txt); // ficheiro .txt
    // pandoc f1.txt

    strcat(command, " -o ");
    // pandoc f1.txt -o

    strcat(command, str_epub);
    // pandoc f1.txt -o f1.epub

    printf("[pid%d", getpid());
    printf("]");
    printf("converting %s\n", txt);
}

int main(int argc, char **argv)
{

    if (argc >= N_ARGS)
    {
        printf("ERROR: few arguments\n");
        printf("EXAMPLE RIGHT COMPILATION: ./samples quote.txt 5 5\n");
        exit(EXIT_FAILURE);
    }




    char command[MAX_STR_SIZE];
    strcpy(command, "");

    char zip_command[MAX_STR_SIZE];
    strcpy(zip_command, "");

    char str_epub[MAX_STR_SIZE]; // nome do ficheiro já no formato pedido .epub
    strcpy(str_epub, "");

    strcat(zip_command, "zip ebooks.zip ");

    pid_t pids[argc - 1];
    int i;
    int n = argc;

    for (size_t i = 1; i <= (argc - 1); i++)
    {

        strcpy(str_epub, argv[i]);
        str_epub[strlen(str_epub) - 3] = 'e';
        str_epub[strlen(str_epub) - 2] = 'p';
        str_epub[strlen(str_epub) - 1] = 'u';
        str_epub[strlen(str_epub) - 0] = 'b';

        strcat(zip_command, str_epub);
        // zip ebooks.zip f1.epub
        strcat(zip_command, " ");
    }

    /* Start children. */
    for (i = 1; i < n; ++i)
    {
        if ((pids[i] = fork()) < 0)
        {
            perror("fork");
            abort();
        }
        else if (pids[i] == 0)
        {

            strcpy(str_epub, argv[i]);
            str_epub[strlen(str_epub) - 3] = 'e';
            str_epub[strlen(str_epub) - 2] = 'p';
            str_epub[strlen(str_epub) - 1] = 'u';
            str_epub[strlen(str_epub) - 0] = 'b';

            // zip ebooks.zip f1.epub

            strcat(command, "pandoc ");
            strcat(command, argv[i]); // ficheiro .txt
            strcat(command, " -o ");
            strcat(command, str_epub);
            strcat(command, " --quiet ");

            system(command);

            printf("[pid%d", getpid());
            printf("]");
            printf("converting %s\n", argv[i]);

            exit(0);
        }
    }

    /* Wait for children to exit. */
    int status;
    pid_t pid;
    while (n > 0)
    {
        pid = wait(&status);
        --n;
    }

    // printf("\n\nZip command = %s\n\n", zip_command);

    strcat(zip_command, " --quiet ");
    system(zip_command);
}