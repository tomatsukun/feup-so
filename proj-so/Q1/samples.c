#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_FILE_NAME 100

int _charcount(char *file_name) // conta quantos caracteres tem o ficheiro
{
    FILE *fp;

    // Character counter (result)
    int count = 0;

    // To store a character read from file
    char c;




    // Open the file
    fp = fopen(file_name, "r");

    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", file_name);
        return 0;
    }

    // Extract characters from file
    // and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))

        // Increment count for this character
        count = count + 1;

    // Close the file
    fclose(fp);

    return count;
}

int _random(int min, int max) // random numero de no intervalo [min, max]
{

    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int _randomchar(char *file) // retorna uma posição qualquer dentro do numero de caracteres que o ficheiro tenha. Função apenas para simplificar o programa código
{
    return _random(1, _charcount(file));
}

int main(int argc, char **argv)
{

    srand(time(0));

    // argv[1] == file_name;
    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        return 0;
    }

    int n; // numero de strings
    int m; // tamanho da string


    n = atoi(argv[2]);
    m = atoi(argv[3]);



    char aux_char;

    for (size_t i = 0; i < n; i++)
    {
        printf(">");
        fseek(fp, _randomchar(argv[1]), SEEK_SET); // primeiro caracter de cada fragmento

        for (size_t i = 0; i < m; i++)
        {
            aux_char = fgetc(fp);

            if (aux_char == '\n' || aux_char == '\r')
            {
                printf(" ");
            }
            else
            {
                printf("%c", aux_char);
            }
        }

        printf("<\n");
    }




    return 0;
}
