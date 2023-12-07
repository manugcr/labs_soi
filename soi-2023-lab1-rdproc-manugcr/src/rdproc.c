/*  TP N1 - SOI - rdproc - Gil Cernich Manuel
 *  Este programa busca informacion del sistema en /proc/ para luego guardarla o imprimirla en pantalla.
 *  Se pueden utilizar distintas opciones para buscar informacion especifica y el programa es capaz de
 *  identificar las flags recibidas para ejecutar las diferentes opciones.
 * 
 *  Para compilar y ejecutar el programa:
 *  $ make all
 *  $ ./rdproc [opciones]
 * 
 *  Algunas de las opciones que se pueden utilizar son:
 * -s: Imprime informacion del sistema.
 * -l: Imprime informacion del disco cada intervalo de tiempo durante una duracion.
 * -p: Imprime informacion de un proceso.
 * -f: Imprime informacion de un proceso.
 * -t: Imprime informacion de un proceso.
 * -j: Guarda la informacion del sistema en formato JSON.
 * -h: Imprime la ayuda.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys_data.h"
#include "disk_load.h"
#include "pid_info.h"

enum FLAGS {s,l,p,f,t,j,h};

int main(int argc, char *argv[])
{
    int flags[7] = {0}; // Initialize all flags to 0.
    int interval = 0, duration = 0, pid = 0; // This are values given by the user in arguments.

    // In every case, the program will try to validate the flags, read and save the arguments.
    // Then after the while, it will check if the flags are valid and execute the corresponding function.
    int option;
    while ((option = getopt(argc, argv, "slpftjh")) != -1)
    {
        switch (option)
        {
        case 's':
            flags[s] = 1;
            break;
        case 'l':
            flags[l] = 1;
            // Test if there are at least two arguments.
            if (optind + 2 <= argc)
            {
                // Try to save this two arguments.
                if (sscanf(argv[optind], "%d", &interval) == 1 && sscanf(argv[optind + 1], "%d", &duration) == 1)
                    optind += 2; // Skip this two arguments to keep scanning for new flags.
                else
                {
                    fprintf(stderr, "Error: Invalid argument for -l flag.\n");
                    invalid_flag();
                }
            }
            else
            {
                fprintf(stderr, "Error: -p flag requires two arguments.\n");
                invalid_flag();
            }
            break;
        case 'p':
            flags[p] = 1;
            // Test if there is at least one argument.
            if (optind + 1 <= argc)
            {
                // Try to save this argument.
                if (sscanf(argv[optind], "%d", &pid) == 1)
                    optind += 1; // Skip this argument to keep scanning for new flags.
                else
                {
                    fprintf(stderr, "Error: Invalid argument for -p flag.\n");
                    invalid_flag();
                }
            }
            else
            {
                fprintf(stderr, "Error: -p flag requires an argument.\n");
                invalid_flag();
            }
            break;
        case 'f':
            flags[f] = 1;
            // Test if there is at least one argument.
            if (optind + 1 <= argc)
            {
                // Try to save this argument.
                if (sscanf(argv[optind], "%d", &pid) == 1)
                    optind += 1; // Skip this argument to keep scanning for new flags.
                else
                {
                    fprintf(stderr, "Error: Invalid argument for -f flag.\n");
                    invalid_flag();
                }
            }
            else
            {
                fprintf(stderr, "Error: -f flag requires an argument.\n");
                invalid_flag();
            }
            break;
        case 't':
            flags[t] = 1;
            // Test if there is at least one argument.
            if (optind + 1 <= argc)
            {
                // Try to save this argument.
                if (sscanf(argv[optind], "%d", &pid) == 1)
                    optind += 1; // Skip this argument to keep scanning for new flags.
                else
                {
                    fprintf(stderr, "Error: Invalid argument for -t flag.\n");
                    invalid_flag();
                }
            }
            else
            {
                fprintf(stderr, "Error: -f flag requires an argument.\n");
                invalid_flag();
            }
            break;
        case 'j':
            flags[j] = 1;
            break;
        case 'h':
            flags[h] = 1;
            break;
        default:
            fprintf(stderr, "Error: Invalid option.\n");
            invalid_flag();
            break;
        }
    }

    // Always print header.
    print_header();

    // Check if there is a better way to do avoid nested ifs.
    if (flags[h])
    {
        print_help();
    }
    else if ((argc == 1) || (argc == 2 && (flags[j] || flags[s])) || (argc == 3 && flags[j] && flags[s]))
    {
        print_system_data(flags[s], flags[j]);
    }
    else if (flags[l] && !flags[s] && !flags[p] && !flags[f] && !flags[t] && !flags[h])
    {
        print_interval(interval, duration, flags[j]);
    }
    else if (flags[p] && !flags[t] && !flags[s] && !flags[l] && !flags[f] && !flags[h])
    { // Not implemented
        print_system_data(flags[s], flags[j]);
        print_pid_info_p(pid, flags[j]);
    }
    else if (flags[f] && !flags[t] && !flags[s] && !flags[l] && !flags[p] && !flags[h])
    {
        print_system_data(flags[s], flags[j]);
        print_pid_info_f(pid, flags[j]);
    }
    else if (flags[t] && !flags[f] && !flags[s] && !flags[l] && !flags[p] && !flags[h])
    { // Not implemented
        print_system_data(flags[s], flags[j]);
        print_pid_info_t(pid, flags[j]);
    }
    else
    {
        fprintf(stderr, "Error: Invalid option.\n");
        invalid_flag();
    }

    return 0;
}