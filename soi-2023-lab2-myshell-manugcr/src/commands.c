#include "commands.h"
#include "external.h"


/*  Exits the terminal.
    quit --> exit(EXIT_SUCCESS)
*/
static void exit_terminal()
{
    exit(EXIT_SUCCESS);
}


/*  Changes current directory and updates the enviroment variable PWD and OLDPWD.
    cd (null) --> /home/username
    cd (..)   --> father directory
    cd (-)    --> previous directory

    chdir() Changes the directory of the process to the directory specified in path.
    getcwd() Copies an absolute pathname of the current working directory.
    The we set the OLDPWD with the actual PWD and the PWD with the new directory got in getcwd().
*/
static void cd(char *directory)
{
    /* directory contains the full lenght command i.e "cd /home/" so we grab only the arg. */
    directory = strtok(NULL, " ");

    if (directory == NULL)
        directory = getenv("HOME");

    if(!strcmp(directory, "-"))
        directory = getenv("OLDPWD");

    if (chdir(directory) == -1)
        printf("cd: %s: No such file or directory\n", directory);

    char* cwd = malloc(sizeof(char)*1024);
    getcwd(cwd, 1024);
    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", cwd, 1);
    free(cwd); 
}


/*  With \033[H\033[J, you are moving the cursor to the top-left corner and then 
    clearing the entire screen, effectively giving you a "clear" effect similar 
    to what system("clear") would do.
*/
static void clr()
{
    printf("\033[H\033[J");
}


/*  Function to print the comment given by the user.
        @param comment: the comment to print.
    If the comment is null, it prints a new line.
    If the comment starts with $ it means that it is a enviroment variable.  
*/
static void echo(char *comment)
{
    /* comments contains the full lenght command i.e "echo hello" so we grab only the arg. */
    comment = strtok(NULL, " ");

    if (comment == NULL)
        printf("\n");
    else if (comment[0] == '$')
        printf("%s\n", getenv(comment+1));
    else
        printf("%s\n", comment);
}


/*  Executes the command given by the user.
        @param command: the command to execute.

    - Command null, it does nothing.
    - Command quit, it exits the terminal.
    - Command cd, it changes the directory.
        @param args: the directory to change.
    - Command clr, it clears the terminal.
    - Command echo, it prints the comment given by the user.
        @param args: the comment to print.
    - Command not found, it executes an external command.

    If there is a `&` in the command line, it should execute all the commands separated
    by the `&` in parallel.
*/
void execute_command(char *command)
{
    /* Clear new lines in the string. */
    command[strcspn(command,"\n")] = '\0';

    /* Separate the command from the args in the command. */
    char *cmd = strdup(command); 
    strtok(cmd, " ");

    if (cmd != NULL)
    {
        if (!strcmp(cmd, "quit"))
            exit_terminal();
        else if (!strcmp(cmd, "cd"))
            cd(command);
        else if (!strcmp(cmd, "clr"))
            clr();
        else if (!strcmp(cmd, "cho"))
            echo(command);
        else
            external_command(command);
    }
}