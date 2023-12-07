# Memory managment
### SOI - FCEFyN - UNC - 2023

## Objetivo
Se debe implementar una librería con las funciones:
```C
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
```
También se debe implentar la función 
```C
void check_heap();
```
que debe imprimir el estado del heap. Esta función ayudara a _debugguear_ el programa.

A fines de evitar fragmentación de memoria y para mayor rendimiento, se debe implementar una lista doblemente enlazada para la gestión de memoria libre.
```C
void malloc_control(int OPTION);    // 0: first fit, 1: best fit
```
#### Valgrind (opcional)
Se debe utilizar el conjunto de herramientas provistas por Valgrind para realizar un análisis dinámico y profiling del trabajo. Documentarlo.
```
$ make main
$ valgrind ./main
```


## Ejecucion
Para compilar y ejecutar el script se puede utilizar el archivo makefile:
```
$ make main
$ ./main
```

Para eliminar el ejecutable utilizar makefile:
```
$ make clean
```

El script no recibe argumentos y su funcionalidad esta dentro del main, por lo que al ejecutarse se puede ver el funcionamiento de las funciones implementadas, y para testear diferentes casos habria que modifcar el codigo.

## Memory Map
**¿Por qué no se utiliza siempre el comando mmap?**
`mmap` es una llamada al sistema que se utiliza para asignar direcciones virtuales en el espacio de direcciones de un proceso, principalmente para mapear archivos en memoria o para asignar regiones de memoria. 

No se utiliza siempre debido a su complejidad en comparacion con funciones mas simples como `malloc`, y a su falta de portabilidad, ya que no es parte de la biblioteca estándar de C. Por eso para entornos con recursos limitados como los sistemas embebidos, `mmap` no siempre es la mejor opcion debido a su mayor consumo de recursos y complejidad.

## TO DO
- Realizar debuggeo con valgrind.