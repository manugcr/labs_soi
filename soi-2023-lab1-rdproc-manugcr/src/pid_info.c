#include "pid_info.h"

/*  Print functions that get called from main.
 *  This is STEP D of the project.
 *  Each function takes a process id (PID) and a flag that indicates whether the output should be stored in JSON format.
 *  They call static get functions to populate the struct with data and then print the values of the struct.
 *  -p gets information about filedescriptors in /proc/pid/fd
 *  -f gets information about process limits in /proc/pid/limits
 *  -t gets information about the stack trace in /proc/pid/stack
 */
// TO DO implement stacktrace.
// TO DO implement json_flag

// Struct to hold process limits data
ProcessLimits processLimits;
FileDescriptors *fileDescriptors;

static void get_descriptor_info(int pid, FileDescriptors **fileDescriptors, int *num_fds)
{
    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/fd", pid);

    DIR *dir = opendir(proc_path);
    if (dir == NULL)
    {
        fprintf(stderr, "\nFailed to open directory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Calculate the required size for fd_path
        size_t path_len = (size_t)snprintf(NULL, 0, "%s/%s", proc_path, entry->d_name) + 1;
        char *fd_path = (char *)malloc(path_len);

        // Construct the full path
        snprintf(fd_path, path_len, "%s/%s", proc_path, entry->d_name);

        struct stat statbuf;
        if (lstat(fd_path, &statbuf) == 0)
        {
            FileDescriptors fd;
            snprintf(fd.perms, sizeof(fd.perms), "%c%c%c%c%c%c%c%c%c%c",
                     (S_ISDIR(statbuf.st_mode)) ? 'd' : '-',
                     (statbuf.st_mode & S_IRUSR) ? 'r' : '-',
                     (statbuf.st_mode & S_IWUSR) ? 'w' : '-',
                     (statbuf.st_mode & S_IXUSR) ? 'x' : '-',
                     (statbuf.st_mode & S_IRGRP) ? 'r' : '-',
                     (statbuf.st_mode & S_IWGRP) ? 'w' : '-',
                     (statbuf.st_mode & S_IXGRP) ? 'x' : '-',
                     (statbuf.st_mode & S_IROTH) ? 'r' : '-',
                     (statbuf.st_mode & S_IWOTH) ? 'w' : '-',
                     (statbuf.st_mode & S_IXOTH) ? 'x' : '-');

            // Determine file type using stat, and fallback to d_type if available
            if (S_ISREG(statbuf.st_mode))
                snprintf(fd.type, sizeof(fd.type), "REG");
            else if (S_ISDIR(statbuf.st_mode))
                snprintf(fd.type, sizeof(fd.type), "DIR");
            else if (S_ISLNK(statbuf.st_mode))
                snprintf(fd.type, sizeof(fd.type), "LNK");
            else if (S_ISFIFO(statbuf.st_mode))
                snprintf(fd.type, sizeof(fd.type), "FIFO");
            else if (S_ISSOCK(statbuf.st_mode))
                snprintf(fd.type, sizeof(fd.type), "SOCK");
            else if (S_ISBLK(statbuf.st_mode))
                snprintf(fd.type, sizeof(fd.type), "BLK");
            else
                snprintf(fd.type, sizeof(fd.type), "UNK");

            snprintf(fd.path, sizeof(fd.path), "%s", fd_path);

            // Save the FileDescriptor in the array
            *fileDescriptors = realloc(*fileDescriptors, (size_t)(*num_fds + 1) * sizeof(FileDescriptors));
            (*fileDescriptors)[*num_fds] = fd;
            (*num_fds)++;

            free(fd_path);
        }
        else
        {
            fprintf(stderr, "\nFailed to get file information: %s\n", strerror(errno));
            free(fd_path);
            exit(EXIT_FAILURE);
        }
    }

    closedir(dir);
}

static void get_limits_info(const char *PID, ProcessLimits *processLimits)
{
    char path[100];
    snprintf(path, sizeof(path), "/proc/%s/limits", PID);
    char *line = get_line_from_id(path, "Max open files");
    strcpy(processLimits->resource, "Max open files");
    strcpy(processLimits->pid, PID);
    sscanf(line, "%*s %*s %*s %s %s %s",
           processLimits->soft_limit,
           processLimits->hard_limit,
           processLimits->units);

    free(line);
}





void print_pid_info_p(int PID, int json_flag)
{
    int num_fds = 0;
    get_descriptor_info(PID, &fileDescriptors, &num_fds);

    // Print the information stored in the array of structs
    printf(WHTBG "\n%-1s %-8s %-15s %-8s %-15s" RESET "\n", "", "PID", "Permits", "Type", "Path");
    for (int i = 0; i < num_fds; i++)
        printf("%-1s %-8d %-15s %-8s %-15s\n", "", PID, fileDescriptors[i].perms, fileDescriptors[i].type, fileDescriptors[i].path);

    if (json_flag)
    {
        cJSON *root = cJSON_CreateObject();
        cJSON *fileDescriptorsArray = cJSON_CreateArray();
        for (int i = 0; i < num_fds; i++)
        {
            cJSON *fileDescriptor = cJSON_CreateObject();
            cJSON_AddItemToObject(fileDescriptor, "Permits", cJSON_CreateString(fileDescriptors[i].perms));
            cJSON_AddItemToObject(fileDescriptor, "Type", cJSON_CreateString(fileDescriptors[i].type));
            cJSON_AddItemToObject(fileDescriptor, "Path", cJSON_CreateString(fileDescriptors[i].path));
            cJSON_AddItemToArray(fileDescriptorsArray, fileDescriptor);
        }
        cJSON_AddItemToObject(root, "File Descriptors", fileDescriptorsArray);

        FILE *file = fopen("./json/pid.json", "w");
        if (file)
        {
            char *json = cJSON_Print(root);
            fputs(json, file);
            fclose(file);
            free(json);
            printf(RESET BOLD "\n* Saved as json in ---> ./json/pid.json *" RESET "\n\n");
        }
    }

    // Free the allocated memory
    free(fileDescriptors);
}

void print_pid_info_f(int PID, int json_flag)
{
    char pid_str[64];
    sprintf(pid_str, "%d", PID);
    
    // Populate the limits struct with data (you can set these values as needed)
    get_limits_info(pid_str, &processLimits);

    // Print the values of the limits struct
    printf(WHTBG "\n%-2s%-10s%-20s%-16s%-16s%-16s" RESET "\n", "", "PID", "Resource", "Soft Limit", "Hard Limit", "Units");
    printf("%-2s", "");
    printf("%-10s", processLimits.pid);
    printf("%-20s", processLimits.resource);
    printf("%-16s", processLimits.soft_limit);
    printf("%-16s", processLimits.hard_limit);
    printf("%-16s\n", processLimits.units);

    if (json_flag)
    {
        cJSON *root = cJSON_CreateObject();

        cJSON_AddItemToObject(root, "PID", cJSON_CreateString(processLimits.pid));
        cJSON_AddItemToObject(root, "Resource", cJSON_CreateString(processLimits.resource));
        cJSON_AddItemToObject(root, "Soft Limit", cJSON_CreateString(processLimits.soft_limit));
        cJSON_AddItemToObject(root, "Hard Limit", cJSON_CreateString(processLimits.hard_limit));
        cJSON_AddItemToObject(root, "Units", cJSON_CreateString(processLimits.units));

        FILE *file = fopen("./json/pid.json", "w");
        if (file)
        {
            char *json = cJSON_Print(root);
            fputs(json, file);
            fclose(file);
            free(json);
            printf(RESET BOLD "\n* Saved as json in ---> ./json/pid.json *" RESET "\n\n");
        }
    }
}

void print_pid_info_t(int PID, int json_flag)
{
    char pid_str[64];
    sprintf(pid_str, "%d", PID);
    printf("stack trace %d %d\n", PID, json_flag);
}