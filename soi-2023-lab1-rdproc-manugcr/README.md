# TP1 SO1 - rdproc
Este trabajo consta en la elaboracion de un programa en lenguaje C el cual lee los archivos de /proc para obtener la informacion solicitada del sistema.

## Ejecucion
Para compilar y ejecutar el script se puede utilizar el archivo makefile:
```
$ make
$ ./rdproc
```

Para eliminar el ejecutable utilizar makefile:
```
$ make clean
```

Se pueden utilizar diferentes flags para obtener diferente informacion:
```
$ ./rdproc -s -j
$ ./rdproc -f 100
```

## Flags.
- Sin flag se imprime informacion basica del sitema.
- `-s` 
    Se agrega informacion a la salida estandar sin flag.
    - Cantidad de tiempo de CPU utilizado para usuarios, sistema y el proceso idle.
    - Cantidad de cambios de contexto.
    - Numero de procesos creados desde el inicio del sistema.
- `-l <interval> <duration>` 
    Imprime estadisticas del disco en un intervalo.
    - Peticiones al disco.
    - Memoria disponible Available / Total.
    - Promedio de carga en el ultimo minuto.
- `-p <pid>` 
    - Imprime la informacion sobre los file descriptors a los que tiene acceso un proceso en particular.
- `-f <pid>`
    - Imprime los límites de archivos abiertos para un cierto proceso.
- `-t <pid>`
    - Imprime el kernel stack trace actual de un cierto proceso.
- `-j`
    - Se puede aplicar con otra flag para guardar la informacion obtenida en formato json.

## Ejemplos de ejecucion.

```c
$ ./rdproc -l 1 3
Disk: 477
Memory: 763652/7999908
Load average: 0.720000
[1 second pause]

Disk: 477
Memory: 756144/7999908
Load average: 0.720000
[1 second pause]

Disk: 477
Memory: 756144/7999908
Load average: 0.720000
```
```
$ ./rdproc -p 1995
Model Name: Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
Vendor ID: GenuineIntel
Kernel: 5.15.0-84-generic
Cores: 2
Threads: 4
Uptime: 00:04:09:21
Partitions: sda sda1 sdb 
Filesystems: sysfs tmpfs bdev 

  PID      Permits         Type     Path           
  1995     -r-x------      LNK      /proc/1995/fd/0
  1995     --wx------      LNK      /proc/1995/fd/1
  1995     --wx------      LNK      /proc/1995/fd/2
  1995     -rwx------      LNK      /proc/1995/fd/3
  1995     -rwx------      LNK      /proc/1995/fd/4
  1995     -rwx------      LNK      /proc/1995/fd/5
  1995     -rwx------      LNK      /proc/1995/fd/6
  1995     -rwx------      LNK      /proc/1995/fd/7
  1995     -rwx------      LNK      /proc/1995/fd/8
  1995     -rwx------      LNK      /proc/1995/fd/9
  1995     -rwx------      LNK      /proc/1995/fd/10
  1995     -rwx------      LNK      /proc/1995/fd/11

```

Ejecutar la flag `-j` te genera un archivo con la informacion pedida y se guarda en ./json/
```c
$ ./rdproc -j
```
```json
{
	"ModelName":	"Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz",
	"VendorID":	"GenuineIntel",
	"Kernel":	"5.15.0-84-generic",
	"Cores":	2,
	"Threads":	4,
	"Uptime":	"00:04:04:48",
	"Filesystems":	["sysfs", "tmpfs", "bdev", "proc", "cgroup"],
	"Partitions":	["sda", "sda1", "sdb"]
}
```



## Consideraciones a la hora de resolverlo.

#### Manejo de json.
Para obtener y poder trabajar con archivos en formato json se utilizo la libreria [cJson](https://github.com/DaveGamble/cJSON).

#### Headers
El codigo esta organizado en 4 archivos diferentes para separar diferentes steps de la resolucion. Al momento de guardar informacion se tienen 3 tipos de json diferentes, uno con informacion del sistema, uno con el polling y otro con informacion de PID en particular.
- **utils.c**
Este archivo contine funciones para leer los archivos de /proc que son utilizadas constantemente por los demas steps. Las funciones de utils.h retornan la linea del file de /proc que se busca, ya sea por una ID o por un numero de linea.
- **sys_data.c**
En este archivo se resuelven el STEP A y STEP B en conjunto ya que son muy similares entre si y uno es una extension del otro. Basicamente se llaman a las funciones de utils.c para obtener informacion necesaria de /proc y se maneja el string para limpiarlo de caracteres innecesarios a la hora de hacer un print. Tambien si se proporciona la flag `-j` se guarda la informacion en json luego de imprimirla dentro de la misma funcion.
La informacion obtenida se guarda en un struct.
Solo hay dos funciones publicas que se llaman desde el main, una para hacer un print del header y otra para obtener la informacion, a la cual se le dan las flags `-s` y `-j` para extender la informacion o para guardar en json.
- **disk_load**
Este archivo resuelve el STEP C, donde se llama a la funcion dandole parametros de intervalo y duracion para hacer un ciclo donde se obtiene la informacion de memoria y carga al disco. Si se proporciona la flag `-j` se guarda este ciclo en un json que es diferente al que se obtiene con datos de sistema. Es decir que este json solo contiene informacion del STEP C.
- **pid_info**
En este archivo se cumplen las diferentes flags del STEP D, donde se necesita obtener diferente informacion de un PID en particular que se proporciona cuando se llama al metodo correspondiente. La informacion tambien puede ser guardada en un json, donde este va a ser independiente de los demas json.

En varias funciones se utiliza un buffer[num] con un tamanio fijo para leer las lineas de un archivo, esto se decidio para no tener que alocar memoria dinamicamente y poder controlar mejor el memory bleeding. De la misma manera los structs son de variables estaticas, menos en los casos que no se podia decidir un tamanio fijo al tener que guardar informacion que puede ser distinta en diferentes terminales.

# TO DO.
- Informacion de cache L1 y L2 desde /proc. (Se puede obtener con lscpu)
- Incorporar las flags `-t` y su respectiva flag `-j`.
- Bug si a la flag `-l` se le dan negativos.
- Hacer la parte de testing.
- Se podrian simplificar funciones, estoy seguro que hay lineas de codigo de mas.
- Mejorar el manejo de memoria, hay partes que no son optimas y hay memory bleeding.
- Revisar si hay secciones donde se pueda llegar a un error no controlado.