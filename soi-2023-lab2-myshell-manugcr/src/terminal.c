#include "terminal.h"


char* username;
char* hostname;
char* current_working_directory;


static void print_header()
{
    printf("\n");
    printf(RED "%-8s %-33s \n","" , " __  __        ___ _        _ _ ");
    printf("%-8s %-33s \n","" , "|  \\/  |_  _  / __| |_  ___| | |");
    printf("%-8s %-33s \n","" , "| |\\/| | || | \\__ \\ ' \\/ -_) | |");
    printf("%-8s %-33s \n","" , "|_|  |_|\\_, | |___/_||_\\___|_|_|");
    printf("%-8s %-33s \n" RESET,"" , "        |__/                    ");
    printf("\n");
}


static void get_username(char** username)
{
    *username = getenv("USER");
}


static void get_hostname(char** hostname)
{
    gethostname(*hostname, 32);
}


static void get_cwd(char** cwd)
{
    *cwd = getenv("PWD");
}


/* Prints the command line user@hostname: <current path>$ */
static void print_line()
{
    /* Refresh variables */
    get_username(&username);
    get_hostname(&hostname);
    get_cwd(&current_working_directory);

    printf("╭─");
    printf(RED "%s@%s" RESET ":", username, hostname);

    /* If cwd is /home/user print "~$" instead of "cwd$" */
    if (strcmp(current_working_directory, getenv("HOME")) == 0)
        printf(RESET "~/\n");
    else
        printf(RESET "%s\n", current_working_directory);
    
    printf("╰─$ ");
}


/* Initialize the variables needed to print in the terminal. */
void init_terminal()
{
    username = (char*)malloc(sizeof(char)*32);
    get_username(&username);

    hostname = (char*)malloc(sizeof(char)*32);
    get_hostname(&hostname);    

    current_working_directory = (char*)malloc(sizeof(char)*64);
    get_cwd(&current_working_directory);

    print_header();
}


/* This functions is inside the main while loop, it prints the terminal line and grabs the command to execute. */
char* get_command()
{
    print_line();

	char* command = (char*)malloc(sizeof(char)*64);
	if(fgets(command, 64, stdin) == NULL)
        exit(EXIT_SUCCESS);

	return command;
}