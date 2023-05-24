#include <stdio.h>


#define DEFAULT_INPUTFILE "prog.asm"
#define DEFAULT_OUTPUTFILE "prog.hex"
#define MAX_LEN_FILENAME 100
#define HELP_TEXT "\
\
\n----------\n\
This is the help menu\n\
In case you haven't invoked this menu, it means your syntax is incorrect\n\
This is an educational software with a free GNU licencing, feel free to use, but mention the authors\n\
Authors: Lucas Pereira do Amaral\
         Gabriela Barbosa Guedes Pereira\n\
\n----------\n\
Avalilable flags:\n\
    -h: shows this help menu\n\
    -f [filename]: indicate the path to the file to be compiled\n\
    -o [filename]: indicate the path to the output hex file\n\
In case you have not specified input or output files, the defaults will ve used\n\
Default input file: prog.asm\n\
Default output file: prog.hex\n\
"

void print_help()
{
    printf(HELP_TEXT);
    return;
}

bool verify_filename(char *supposed_path, char **file_path)
{   // check path for safety
    char *path = (char*)malloc(MAX_LEN_FILENAME*sizeof(char));
    
    for (int i=0; i<MAX_LEN_FILENAME; i++)
    {
        path[i] = supposed_path[i];

        if (path[i]=='\0') break;
        if (i==MAX_LEN_FILENAME-1) path[0]='\0'; 
    }

    if (path[0]=='\0')
    {
        printf("[SET_FILENAME] Filename path might be too long or invalid\n");
        return false;
    }
    printf("[SET_FILENAME] Path set as: %s\n", path);
    *file_path = path;
    return true;
}

int initializeFiles(int argc, char *argv[], char **inputfile, char **outputfile)
{
    char *tmp1 = (char*)DEFAULT_INPUTFILE;
    char *tmp2 = (char*)DEFAULT_OUTPUTFILE;
    *inputfile = tmp1;
    *outputfile = tmp2;

    int args=1;
    while (args < argc)
    {
        if (argv[args][0]=='-')
        {
            switch (argv[args][1])
            {
            case 'h':
                print_help(); // print_help();
                break;
            case 'f':
                args++;
                if (args<argc) 
                { 
                    if (!verify_filename(argv[args], inputfile)) return 403; 
                    printf("[MANAGESTART] InputFile updated to %s\n", *inputfile);
                }
                else 
                {
                    printf("[MANAGESTART]Your [filename] argument is missing\n");
                    return 404;
                }
                break;
            case 'o':
                args++;
                if (args<argc) 
                { 
                    if (!verify_filename(argv[args], outputfile)) return 403; 
                    printf("[MANAGESTART] OutputFile updated to %s\n", *outputfile);
                }
                else 
                {
                    printf("[MANAGESTART]Your [filename] argument is missing\n");
                    return 404;
                }
                break;
            default:
                print_help();
                break;
            }
        }
        else print_help();
        args++;
    }
    printf("[MANAGESTART] Sucessful exit\n\n");
    return 0;
}