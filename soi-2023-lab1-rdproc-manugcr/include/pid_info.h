#ifndef PID_INFO_H
#define PID_INFO_H
#include "utils.h"
#include "cJSON.h"

typedef struct
{
    char pid[32];        // Store PID
    char resource[32];   // Store resource names
    char soft_limit[32]; // Store soft limits
    char hard_limit[32]; // Store hard limits
    char units[32];      // Number of resource limits
} ProcessLimits;

typedef struct
{
    char perms[11];
    char type[5];
    char path[256];
} FileDescriptors;

void print_pid_info_p(int PID, int json_flag);
void print_pid_info_f(int PID, int json_flag);
void print_pid_info_t(int PID, int json_flag);

#endif