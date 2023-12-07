#include "sys_data.h"

/* Functions to get data from the system. This is STEP A and STEP B of the project.
 * The information is stored in a struct and it can be printed or saved as a json file.
 * There is also the header function that prints at the begining of the script that gets its info from the struct.
 * print_system_data: prints the system data and receives two flags.
 *  By default prints basic information.
 *  - s_flag: flag -s to print system processes data.
 *  - j_flag: flag -j to save the output as a json file.
 */

SystemInfo systemInfo;

// Header.
static void set_user(SystemInfo *sysInfo)
{
    sysInfo->username = getenv("USER");
}

static void set_hostname(SystemInfo *sysInfo)
{
    sysInfo->hostname = get_line_from_num("/proc/sys/kernel/hostname", 1);
}

static void set_time(SystemInfo *sysInfo)
{
    char *time = get_line_from_id("/proc/driver/rtc", "rtc_time");
    sysInfo->current_time = strtok(time, " ") + strlen("rtc_time") + 3;
}

static void set_date(SystemInfo *sysInfo)
{
    char *date = get_line_from_id("/proc/driver/rtc", "rtc_date");
    sysInfo->current_date = strtok(date, " ") + strlen("rtc_date") + 3;
}

// STEP A. No flag

static void set_model_name(SystemInfo *sysInfo)
{
    sysInfo->model_name = get_line_from_id("/proc/cpuinfo", "model name") + strlen("model name") + 3;
}

static void set_vendor_id(SystemInfo *sysInfo)
{
    sysInfo->vendor_id = get_line_from_id("/proc/cpuinfo", "vendor_id") + strlen("vendor_id") + 3;
}

static void set_kernel(SystemInfo *sysInfo)
{
    sysInfo->kernel = get_line_from_num("/proc/sys/kernel/osrelease", 1);
}

static void set_cores(SystemInfo *sysInfo)
{
    sysInfo->cores = atoi(get_line_from_id("/proc/cpuinfo", "cpu cores") + strlen("cpu cores") + 3);
}

static void set_threads(SystemInfo *sysInfo)
{
    sysInfo->threads = atoi(get_line_from_id("/proc/cpuinfo", "siblings") + strlen("siblings") + 3);
}

static void set_uptime(SystemInfo *sysInfo)
{
    char *uptime = strdup(strtok(get_line_from_num("/proc/uptime", 1), " "));
    sysInfo->uptime = get_formatted_time(uptime);
}

// static void set_partitions(SystemInfo *sysInfo)
// {
//     sysInfo->num_partitions = get_num_lines("/proc/mounts");

//     // Allocate memory for partitons
//     sysInfo->partitions = malloc((size_t)sysInfo->num_partitions * sizeof(PartitonsInfo));

//     for (int i = 0; i < sysInfo->num_partitions; ++i)
//     {
//         char *line = get_line_from_num("/proc/mounts", i);

//         // Allocate memory for partiton and filesystem fields
//         sysInfo->partitions[i].device[0] = '\0'; // Initialize as empty string
//         sysInfo->partitions[i].mount[0] = '\0';  // Initialize as empty string
//         sysInfo->partitions[i].filesystem[0] = '\0';

//         // Use the arrays themselves, not their addresses
//         sscanf(line, "%31s %*s", sysInfo->partitions[i].device);
//         sscanf(line, "%*s %63s", sysInfo->partitions[i].mount);
//         sscanf(line, "%*s %*s %31s", sysInfo->partitions[i].filesystem);

//         free(line); // Free the memory allocated by get_line_from_num
//     }
// }
static void set_partitions(SystemInfo *sysInfo)
{
    sysInfo->num_partitions = get_num_lines("/proc/partitions") - 2;

    // Allocate memory for partitons
    sysInfo->partitions = malloc((size_t)sysInfo->num_partitions * sizeof(PartitonsInfo));

    for (int i = 1; i < sysInfo->num_partitions; i++)
    {
        char *line = get_line_from_num("/proc/partitions", i+2);

        // Allocate memory for partiton and filesystem fields
        sysInfo->partitions[i].partition[0] = '\0'; // Initialize as empty string

        // Use the arrays themselves, not their addresses
        sscanf(line, "%*d %*d %*d %31s", sysInfo->partitions[i].partition);

        free(line); // Free the memory allocated by get_line_from_num
    }
}


static void set_filesystems(SystemInfo *sysInfo)
{
    sysInfo->num_fs = get_num_lines("/proc/filesystems");
    // Allocate memory for filesystem
    sysInfo->filesystems = malloc((size_t)sysInfo->num_fs * sizeof(FileSystemsInfo));

    int index = 0; // Index to track valid filesystem entries
    for (int i = 0; i < sysInfo->num_fs; ++i)
    {
        char *line = get_line_from_num("/proc/filesystems", i);

        // Check if the line starts with "nodev"
        if (strncmp(line, "nodev", 5) != 0)
        {
            // Line does not start with "nodev," process it
            strcpy(sysInfo->filesystems[index].type, "");
            sscanf(line, "%31s", sysInfo->filesystems[index].filesystem);
            index++; 
        } else {
            // Line starts with "nodev," save it as "nodev"
            
            sscanf(line, "%31s %31s", sysInfo->filesystems[index].type, sysInfo->filesystems[index].filesystem);
            index++;
        }

        free(line); // Free the memory allocated by get_line_from_num
    }
    sysInfo->num_fs = index; // Update the number of valid filesystem entries
}


// STEP B. Flag -s
static void set_user_processes(SystemInfo *sysInfo)
{
    char *line = get_line_from_id("/proc/stat", "cpu");
    sscanf(line, "%*s %d", &sysInfo->user_processes);
}

static void set_system_processes(SystemInfo *sysInfo)
{
    char *line = get_line_from_id("/proc/stat", "cpu");
    sscanf(line, "%*s %*d %*d %d", &sysInfo->system_processes);
}

static void set_idle_processes(SystemInfo *sysInfo)
{
    char *line = get_line_from_id("/proc/stat", "cpu");
    sscanf(line, "%*s %*d %*d %*d %d", &sysInfo->idle_processes);
}

static void set_ctxt_switches(SystemInfo *sysInfo)
{
    sysInfo->ctxt_switches = atoi(get_line_from_id("/proc/stat", "ctxt") + strlen("ctxt") + 1);
}

static void set_processes(SystemInfo *sysInfo)
{
    sysInfo->processes = atoi(get_line_from_id("/proc/stat", "processes") + strlen("processes") + 1);
}

/*  Functions to set data by blocks.
 *  They use all the above functions to save the data to the struct.
 */

// Header of the script.
static void set_header_data(SystemInfo *sysInfo)
{
    set_user(sysInfo);
    set_hostname(sysInfo);
    set_date(sysInfo);
    set_time(sysInfo);
}

// STEP A. No flag
static void set_system_data(SystemInfo *sysInfo)
{
    set_model_name(sysInfo);
    set_vendor_id(sysInfo);
    set_kernel(sysInfo);
    set_cores(sysInfo);
    set_threads(sysInfo);
    set_uptime(sysInfo);
    set_partitions(sysInfo);
    set_filesystems(sysInfo);
}

// STEP B. Flag -s
static void set_processes_data(SystemInfo *sysInfo)
{
    set_user_processes(sysInfo);
    set_system_processes(sysInfo);
    set_idle_processes(sysInfo);
    set_ctxt_switches(sysInfo);
    set_processes(sysInfo);
}

static void save_to_json(int s_flag)
{
    cJSON *root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "ModelName", systemInfo.model_name);
    cJSON_AddStringToObject(root, "VendorID", systemInfo.vendor_id);
    cJSON_AddStringToObject(root, "Kernel", systemInfo.kernel);
    cJSON_AddNumberToObject(root, "Cores", systemInfo.cores);
    cJSON_AddNumberToObject(root, "Threads", systemInfo.threads);
    cJSON_AddStringToObject(root, "Uptime", systemInfo.uptime);

    if (s_flag)
    {
        cJSON_AddNumberToObject(root, "UserProcesses", systemInfo.user_processes);
        cJSON_AddNumberToObject(root, "SystemProcesses", systemInfo.system_processes);
        cJSON_AddNumberToObject(root, "IdleProcesses", systemInfo.idle_processes);
        cJSON_AddNumberToObject(root, "ContextSwitches", systemInfo.ctxt_switches);
        cJSON_AddNumberToObject(root, "Processes", systemInfo.processes);
    }

    cJSON *filesystems = cJSON_CreateArray();
    for (int i = 1; i < systemInfo.num_fs; ++i)
    {
        cJSON_AddItemToArray(filesystems, cJSON_CreateString(systemInfo.filesystems[i].filesystem));
    }
    cJSON_AddItemToObject(root, "Filesystems", filesystems);

    cJSON *partitions = cJSON_CreateArray();
    for (int i = 1; i < systemInfo.num_partitions; ++i)
    {
        cJSON_AddItemToArray(partitions, cJSON_CreateString(systemInfo.partitions[i].partition));
    }
    cJSON_AddItemToObject(root, "Partitions", partitions);


    // Old approach
    // cJSON *filesystems = cJSON_CreateArray();
    // for (int i = 1; i < systemInfo.num_fs; ++i)
    // {
    //     cJSON *filesystem = cJSON_CreateObject();
    //     cJSON_AddStringToObject(filesystem, "Type", systemInfo.filesystems[i].type);
    //     cJSON_AddStringToObject(filesystem, "Filesystem", systemInfo.filesystems[i].filesystem);
    //     cJSON_AddItemToArray(filesystems, filesystem);
    // }
    // cJSON_AddItemToObject(root, "Filesystems", filesystems);

    // cJSON *partitions = cJSON_CreateArray();
    // for (int i = 1; i < systemInfo.num_partitions; ++i)
    // {
    //     cJSON *partition = cJSON_CreateObject();
    //     cJSON_AddStringToObject(partition, "Device", systemInfo.partitions[i].device);
    //     cJSON_AddStringToObject(partition, "Filesystem", systemInfo.partitions[i].filesystem);
    //     cJSON_AddStringToObject(partition, "Mount", systemInfo.partitions[i].mount);
    //     cJSON_AddItemToArray(partitions, partition);
    // }
    // cJSON_AddItemToObject(root, "Partitions", partitions);

    printf("\n");

    // Save the JSON string to a file named "output.json"
    FILE *file = fopen("./json/data.json", "w");
    if (file)
    {
        char *json = cJSON_Print(root);
        fputs(json, file);
        fclose(file);
        free(json);
        printf(RESET BOLD "* Saved as json in ---> ./json/data.json *" RESET "\n\n");
    }
}

// Non static functions.

void print_system_data(int s_flag, int j_flag)
{
    set_system_data(&systemInfo);
    
    printf(GREEN "Model Name: " RESET "%s\n", systemInfo.model_name);
    printf(GREEN "Vendor ID: " RESET "%s\n", systemInfo.vendor_id);
    printf(GREEN "Kernel: " RESET "%s\n", systemInfo.kernel);
    printf(GREEN "Cores: " RESET "%d\n", systemInfo.cores);
    printf(GREEN "Threads: " RESET "%d\n", systemInfo.threads);
    printf(GREEN "Uptime: " RESET "%s\n", systemInfo.uptime);

    if (s_flag)
    {
        set_processes_data(&systemInfo);
        printf(GREEN "User Processes: " RESET "%d\n", systemInfo.user_processes);
        printf(GREEN "System Processes: " RESET "%d\n", systemInfo.system_processes);
        printf(GREEN "Idle Processes: " RESET "%d\n", systemInfo.idle_processes);
        printf(GREEN "Context Switches: " RESET "%d\n", systemInfo.ctxt_switches);
        printf(GREEN "Processes: " RESET "%d\n", systemInfo.processes);
    }

    printf(GREEN "Partitions: " RESET);
    for (int i = 1; i < systemInfo.num_partitions; i++)
        printf("%s ", systemInfo.partitions[i].partition);
    printf("\n");

    printf(GREEN "Filesystems: " RESET);
    for (int i = 1; i < systemInfo.num_fs; i++)
        printf("%s ", systemInfo.filesystems[i].filesystem);
    printf("\n");

    // Old approach
    // printf(WHTBG "\n%-1s %-8s %-15s" RESET "\n", "", "type", "filesystem");
    // for (int i = 1; i < systemInfo.num_fs; ++i)
    //     printf("%-1s %-8s %-15s\n", "", systemInfo.filesystems[i].type, systemInfo.filesystems[i].filesystem);

    // printf(WHTBG "\n%-1s %-14s %-20s %-42s" RESET "\n", "", "dev", "filesystem", "mounted");
    // for (int i = 1; i < systemInfo.num_partitions; ++i)
    //     printf("%-1s %-14s %-20s %-42s\n", "", systemInfo.partitions[i].device, systemInfo.partitions[i].filesystem, systemInfo.partitions[i].mount);

    if (j_flag)
        save_to_json(s_flag);
    
}

void print_header()
{
    set_header_data(&systemInfo);

    printf(GREEN "%-50s" RESET BOLD "\n", "                                      ");
    printf(GREEN "%-50s" RESET BOLD "\n", "               _                      ");
    printf(GREEN "%-50s" RESET BOLD "%s\n", "              | |                     ", "SO1");
    printf(GREEN "%-50s"
                 "%s" RESET BOLD "@" GREEN "%s" RESET BOLD "\n",
           "       _ __ __| |_ __  _ __ ___   ___ ", systemInfo.hostname, systemInfo.username);
    printf(GREEN "%-50s" RESET BOLD "%s\n", "      | '__/ _` | '_ \\| '__/ _ \\ / __|", systemInfo.current_date);
    printf(GREEN "%-50s" RESET BOLD "%s\n", "      | | | (_| | |_) | | | (_) | (__ ", systemInfo.current_time);
    printf(GREEN "%-50s" RESET BOLD "\n", "      |_|  \\__,_| .__/|_|  \\___/ \\___|");
    printf(GREEN "%-50s" RESET BOLD "%s\n", "                | |                   ", "-h to get help");
    printf(GREEN "%-50s" RESET BOLD "\n", "                |_|                   ");
    printf(GREEN "%-50s" RESET BOLD "\n", "                                      ");
    printf(GREEN "%-50s" RESET "\n", "                                      ");
}
