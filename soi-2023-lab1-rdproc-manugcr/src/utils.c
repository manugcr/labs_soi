#include "utils.h"

/* General purpouse functions that return information inside an specific file.
 *  - char* get_line_from_id: returns the line that contains the id. 9
 *  - char* get_line_from_num: given a line number, returns the line.
 *  - char* get_num_lines: returns the number of lines in a file.
 *  - char* get_formatted_time: returns the time in a formatted way DD:HH:MM:SS.
 *  - void remove_newline: removes the newline character from the end of a string.
 *     
 * There is also main functions to print help or flag error handling 
 *  - void print_help: prints the help and exits.
 *  - void invalid_flag: prints an error message and exits.
 * 
 * 
*/

void remove_newline(char *value)
{
    char *newline = strchr(value, '\n');
    if (newline != NULL)
        *newline = '\0';
}

// Searchs for and specific id in a file and returns the line that contains it.
// The caller is responsible for freeing the memory of the returned pointer.
char *get_line_from_id(const char *filename, const char *id)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file '%s': %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    char *result = NULL;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (strstr(buffer, id))
        {
            remove_newline(buffer);
            result = strdup(buffer);
            if (result == NULL)
            {
                fprintf(stderr, "Error: Unable to allocate memory for the result\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    if (ferror(file))
    {
        fprintf(stderr, "Error reading from file '%s': %s\n", filename, strerror(errno));
        free(result);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return result;
}


// Jumps to a specific line in a file and returns it.
// In this functions the caller is responsible for freeing the memory of the returned pointer.
char *get_line_from_num(const char *filename, const int linenum)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: %s '%s'\n", strerror(errno), filename);
        exit(EXIT_FAILURE);  
    }

    char buffer[1024];
    char *result = NULL;
    for (int i = 0; i < linenum; ++i)
    {
        if (fgets(buffer, sizeof(buffer), file) == NULL)
        {
            if (feof(file))
                fprintf(stderr, "Error: line overflow '%s'\n", filename);
            else
                fprintf(stderr, "Error reading line %d from '%s': %s\n", linenum, filename, strerror(errno));

            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    remove_newline(buffer);
    fclose(file);

    result = strdup(buffer);
    if (result == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory for the result\n");
        exit(EXIT_FAILURE);
    }

    return result;
}


// Returns the number of lines in a file.
int get_num_lines(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file '%s': %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int num_lines = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
        num_lines++;

    if (ferror(file))
    {
        fprintf(stderr, "Error reading from file '%s': %s\n", filename, strerror(errno));
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return num_lines;
}


// Formats time from /proc/uptime in seconds to DD:HH:MM:SS.
char *get_formatted_time(const char *time_str)
{
    double time_seconds = atof(time_str);    // Convert the input string to a double
    long total_seconds = (long)time_seconds; // Convert to long integer

    long days, hours, minutes, seconds;
    char *formatted_time = (char *)malloc(12); // "DD:HH:MM:SS\0"

    days = total_seconds / (24 * 3600); // Calculate days
    total_seconds %= (24 * 3600);
    hours = total_seconds / 3600; // Calculate hours
    total_seconds %= 3600;
    minutes = total_seconds / 60; // Calculate minutes
    seconds = total_seconds % 60; // Calculate seconds

    // Format the result as "DD:HH:MM:SS" into the allocated memory
    snprintf(formatted_time, 12, "%02ld:%02ld:%02ld:%02ld", days, hours, minutes, seconds);

    return formatted_time;
}

void print_help()
{
    printf(RESET BOLD "rdproc -s /-j" RESET "\n");
    printf("    Displays information about CPU time, context switches, and process numbers.\n");
    printf("    You can specify the -j flag to save the information in JSON format.\n\n");
    printf(RESET BOLD "rdproc -l <i> <d> /-j" RESET "\n");
    printf("    For example, \"rdproc -l 2 100\" will display the 1-minute load average for 100\n");
    printf("    seconds, sampling at 2-second intervals.\n");
    printf("    You can specify the -j flag to save the information in JSON format.\n\n");
    printf(RESET BOLD "rdproc -p <pid> /-j" RESET "\n");
    printf("    Displays information about the process with the specified PID.\n");
    printf("    You can specify the -j flag to save the information in JSON format.\n\n");
    printf(RESET BOLD "rdproc -f <pid> /-j" RESET "\n");
    printf("    Displays information about the process with the specified PID.\n");
    printf("    You can specify the -j flag to save the information in JSON format.\n\n");
    printf(RESET BOLD "rdproc -t <pid> /-j" RESET "\n");
    printf("    Displays information about the stacktrace of the specified PID.\n");
    printf("    You can specify the -j flag to save the information in JSON format.\n\n");
    printf(RESET BOLD "rdproc -h" RESET "\n");
    printf("    Displays the list of allowed flags.\n\n");
    exit(EXIT_FAILURE);
}

void invalid_flag()
{
    fprintf(stderr, "Error: Invalid flag use -h to get help.\n");
    exit(EXIT_FAILURE);
}

