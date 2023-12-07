#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Terminals colors. Keep in mind they can fail on another terminal.
#define RED "\033[1;91m"
#define YELLOW "\033[1;93m"   
#define RESET "\033[0m"

void init_terminal();
char* get_command();

#endif