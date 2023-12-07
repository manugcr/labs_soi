#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"
#include "commands.h"

int main(int argc, char const *argv[])
{
    system("clear");

    /* Initialize variables for command line prompt and print header. */
    init_terminal(); 

    /* If a batchfile is given as argument, execute the commands inside it. */
    if (argc >= 2)
    {
        FILE* batchfile = fopen(argv[1], "r");
        if (batchfile == NULL)
        {
            fprintf(stderr, "Error opening file\n");
            exit(EXIT_FAILURE);
        }

        char* command = (char*)malloc(sizeof(char)*1024);
        while(fgets(command, 1024, batchfile) != NULL)
            execute_command(command);
        fclose(batchfile);
        free(command); 
        exit(EXIT_SUCCESS);
    }

    /* Grab the command and execute it every time the user sends one. */
    while(1)
    {
        char* command = get_command();
        execute_command(command);
    }

    return 0;
}
