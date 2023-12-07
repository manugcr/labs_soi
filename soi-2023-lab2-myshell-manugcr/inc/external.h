#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <signal.h>     // Include the header for signal
#include <sys/types.h>  // Include the header for pid_t
#include <sys/wait.h>   // Include the header for wait

void external_command(char *command);

#endif