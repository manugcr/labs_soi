#ifndef SYS_DATA_H
#define SYS_DATA_H
#include "utils.h"

typedef struct
{
    char type[32];
    char filesystem[32];
} FileSystemsInfo;

typedef struct
{
    // Old approach
    // char device[32]; 
    // char mount[64];
    // char filesystem[32];
    char partition[32];
} PartitonsInfo;

typedef struct
{
    char *hostname;               // Header
    char *username;               // Header
    char *current_date;           // Header
    char *current_time;           // Header
    char *model_name;             // STEP_A
    char *vendor_id;              // STEP_A
    char *kernel;                 // STEP_A
    int cores;                    // STEP_A
    int threads;                  // STEP_A
    char *uptime;                 // STEP_A
    int user_processes;           // STEP_B
    int system_processes;         // STEP_B
    int idle_processes;           // STEP_B
    int ctxt_switches;            // STEP_B
    int processes;                // STEP_B
    PartitonsInfo *partitions;    // STEP A
    int num_partitions;           // STEP B
    FileSystemsInfo *filesystems; // STEP A
    int num_fs;                   // STEP B
} SystemInfo;

void print_header();
void print_system_data(int s_flag, int j_flag);

#endif