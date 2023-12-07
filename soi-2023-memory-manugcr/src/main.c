#include <stdio.h>
#include "memory.h"


int main() 
{
     /*
     * PRUEBA FIRST FIT 
     *
     */
    printf("Prueba MALLOC FIRST FIT\n");
	malloc_control(FIRST_FIT);	
	char *char1 = NULL;
    /* Alocamos memoria */
	char1 = malloc(32);
    /* Guardamos strings en la memoria */
	strcpy(char1, "str 1");
	check_heap(char1);

    /*
     * PRUEBA BEST FIT 
     *
     */
    printf("\n\n\n\nPrueba MALLOC BEST FIT\n");
	malloc_control(BEST_FIT);	
	char *char2 = NULL;
    /* Alocamos memoria */
	char2 = malloc(16);
    /* Guardamos strings en la memoria */
	strcpy(char2, "str 2");
	check_heap(char2);

    /*
     * PRUEBA CALLOC
     *
     */
    printf("\n\n\n\nPrueba CALLOC\n");
	char *char3 = NULL;
    /* Alocamos memoria */
    char3 = calloc(64, sizeof(char));
    /* Guardamos strings en la memoria */
    strcpy(char3, "str 3");
    check_heap(char3);

    /*
     * PRUEBA REALLOC
     *
     */
    printf("\n\n\n\nPrueba REALLOC\n");
    char *char4 = NULL;
    /* Alocamos memoria */
    char4 = realloc(char4, 128);
    /* Guardamos strings en la memoria */
    strcpy(char4, "str 4");
    check_heap(char4);

    free(char1);
    free(char2);
    free(char3);
    free(char4);
    check_heap(char1);
    check_heap(char2);
    check_heap(char3);
    check_heap(char4);
    
    return 0;
}