#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <sys/types.h>  // Include the header for pid_t
#include <sys/wait.h>   // Include the header for wait

void execute_command(char *command);

#endif