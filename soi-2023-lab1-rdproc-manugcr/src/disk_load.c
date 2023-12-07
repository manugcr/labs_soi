#include "disk_load.h"

/* Print functions that get called from main.
 * This is STEP C of the project.
 * Takes an interval and a duration and prints the disk petitions, free memory, total memory and load average.
 * If the j_flag is set, it also saves the output as a json file.
 * The information is not stored, it gets printed or directly save to a json file.
*/

static int get_disk_petitions()
{
    char *petitions = strtok(get_line_from_id("/proc/diskstats", "sda"), "sda");
    petitions = strtok(NULL, " ");
    petitions = strtok(NULL, " ");
    return atoi(strdup(petitions));
}

static int get_free_memory()
{
    return atoi(get_line_from_id("/proc/meminfo", "MemFree") + strlen("MemFree") + 4);
}

static int get_total_memory()
{
    return atoi(get_line_from_id("/proc/meminfo", "MemTotal") + strlen("MemTotal") + 4);
}

static double get_load_avg()
{
    char *line = get_line_from_num("/proc/loadavg", 1);
    sscanf(line, "%s", line);
    return atof(line);
}

void print_interval(int interval, int duration, int j_flag)
{
    int elapsed_time = 0;
    printf("\n");

    cJSON *interval_array = cJSON_CreateArray();

    while (elapsed_time < duration)
    {
        printf(GREEN "Disk: " RESET "%d\n", get_disk_petitions());
        printf(GREEN "Memory: " RESET "%d/%d\n", get_free_memory(), get_total_memory());
        printf(GREEN "Load average: " RESET "%lf\n", get_load_avg());

        cJSON *disk_info = cJSON_CreateObject();
        cJSON_AddNumberToObject(disk_info, "DiskPetitions", get_disk_petitions());
        cJSON_AddNumberToObject(disk_info, "FreeMem", get_free_memory());
        cJSON_AddNumberToObject(disk_info, "TotMem", get_total_memory());
        cJSON_AddNumberToObject(disk_info, "LoadAvg", get_load_avg());

        cJSON_AddItemToArray(interval_array, disk_info);

        elapsed_time += interval;
        if (elapsed_time < duration)
        {
            printf(RESET BOLD "[%d second pause]" RESET "\n\n", interval);
            sleep((unsigned int)interval);
        }
    }
    printf("\n");

    if (j_flag)
    {
        FILE *file = fopen("./json/interval.json", "w");
        if (file)
        {
            char *json = cJSON_Print(interval_array);
            fputs(json, file);
            fclose(file);
            free(json);
            printf(RESET BOLD "* Saved as json in ---> ./json/interval.json *" RESET "\n\n");
        }
    }
}