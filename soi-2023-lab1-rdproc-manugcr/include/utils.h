#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include "cJSON.h"

// Terminals colors. Keep in mind they can fail on another terminal.
#define GREEN "\033[1;92m"  
#define BOLD "\033[1m"
#define WHTBG "\033[7;97m"
#define RESET "\033[0m"

void remove_newline(char *line);
int get_num_lines(const char *filename);
char *get_line_from_num(const char *filename, const int linenum);
char *get_line_from_id(const char *filename, const char *id);
char *get_formatted_time(const char *format);
void print_help();
void invalid_flag();

#endif