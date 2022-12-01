#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_STR_SIZE 64



//system("touch teste.txt");

int main(int argc, char **argv){


    char command[MAX_STR_SIZE];
    strcpy(command, "");
    
    char zip_command[MAX_STR_SIZE];
    strcpy(zip_command, "");
    
    
    char str_epub[MAX_STR_SIZE]; // nome do ficheiro já no formato pedido .epub
    strcpy(str_epub, "");


    strcat(zip_command, "zip ebooks.zip ");
    // zip ebooks.zip 
    



    for (size_t i = 1; i <= (argc-1); i++)
    {
        
        strcpy(str_epub, argv[i]);
        str_epub[strlen(str_epub) - 3] = 'e' ;
        str_epub[strlen(str_epub) - 2] = 'p' ;
        str_epub[strlen(str_epub) - 1] = 'u' ;
        str_epub[strlen(str_epub) - 0] = 'b' ;

        strcat(zip_command, str_epub);
        // zip ebooks.zip f1.epub 
        strcat(zip_command, " ");

        
        strcat(command, "pandoc ");
        strcat(command, argv[i]); // ficheiro .txt
        strcat(command, " -o ");
        strcat(command, str_epub);
        

        system(command); 
        
        /*

        printf("\n");

        printf("Comando f%d = %s\n",i, command);
        
        printf("\n");

        strcpy(command, "");
        
        */

    }
    
    system(zip_command);

    
    /*

    printf("\n");

    printf("Zip comando = %s\n", zip_command);
        
    printf("\n");   

    */

    return 0;
}



