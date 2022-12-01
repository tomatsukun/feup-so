#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STR_SIZE 64




int main(int argc, char* argv[]){


    char aux[MAX_STR_SIZE];


    stpcpy(aux, argv[1]);

    for (size_t i = 0; i < strlen(aux) + 1; i++)
    {
        aux[i] = toupper(aux[i]);
    }
    
    
    printf("%s", aux);
    




}