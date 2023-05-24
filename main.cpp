#include <stdio.h>
#include <stdlib.h>
#include "flags.cpp"
#include "files.cpp"



void apontador(char **);


int main(int argc, char *argv[])
{
    char *inputfile=NULL;
    char *outputfile=NULL;

    printf("[MAIN] Calling initializeFiles.\n\n");
    int error = initializeFiles(argc, argv, &inputfile, &outputfile);
    if (error!=0) return error;
    
    error = compile(inputfile, outputfile);
    if (error!=0) return error;

    free(inputfile);
    free(outputfile);
    return 0;
}

void apontador(char **apon)
{
    // char *tmp = (char*)"mudou";
    // *apon = tmp;
    // printf("File ptr: %d\n", &tmp);
    printf("Inputfile: %s\n", *apon);
    printf("Inputfile addr: %d\n", apon);
    printf("Inputfile ptr: %d\n", *apon);
    printf("Inputfile ptr ptr: %d\n\n", **apon);
    return;
}