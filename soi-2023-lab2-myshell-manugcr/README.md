# MyShell
### SOI - FCEFyN - UNC - 2023


## Objetivo
Los objetivos del presente laboratorio son:
- Utilizar mecanismos de creación de Procesos.
- Utilizar mecanismos de concurrencia e IPC.
- Diseñar un intérprete de línea de comandos al estilo Bourne shell.
- Ampliar las características de un intérprete de línea de comandos al estilo Bourne shell.
- Comprender el uso de la API de señales del SO

##### ¿Dónde se encuentran los pipes en el filesystem, qué atributos tienen?
Los pipes no tienen una representacion en el sistema de archivos como los archivos regulares o directorios, estos son un mecanismo de comunicacion interproceso (IPC) que permite la transferencia de datos entre dos procesos.

No tienen atributos en el sentido de los archivos en el sistema de archivos, pero estan asociados con descriptores de archivo de lectura y escritura. El tamaño del bufer del kernel limita la capacidad de los pipes.

## Ejecucion
Para compilar y ejecutar el script se puede utilizar el archivo makefile:
```
$ make myshell
$ ./myshell
```

Para eliminar el ejecutable utilizar makefile:
```
$ make clean
```

#### Ejemplo de ejecucion
Ejecucion normal.
```
manu@manu:~$ make myshell
manu@manu:~$ ./myshell

          __  __        ___ _        _ _   
         |  \/  |_  _  / __| |_  ___| | |  
         | |\/| | || | \__ \ ' \/ -_) | |  
         |_|  |_|\_, | |___/_||_\___|_|_|  
                 |__/                      

╭─manu@manu:~/
╰─$ echo $SHELL
/bin/bash
```


```
manu@manu:~$ make myshell
manu@manu:~$ ./myshell

          __  __        ___ _        _ _   
         |  \/  |_  _  / __| |_  ___| | |  
         | |\/| | || | \__ \ ' \/ -_) | |  
         |_|  |_|\_, | |___/_||_\___|_|_|  
                 |__/                      

╭─manu@manu:~/
╰─$ ps aux | grep manu
avahi        899  0.0  0.0   7712  3556 ?        Ss   07:56   0:00 avahi-daemon: running [manu.local]
manu        1562  0.0  0.1  17428  9824 ?        Ss   07:56   0:00 /lib/systemd/systemd --user
manu        1563  0.0  0.0 106196  4428 ?        S    07:56   0:00 (sd-pam)
```




```
manu@manu:~$ make myshell
manu@manu:~$ ./myshell

          __  __        ___ _        _ _   
         |  \/  |_  _  / __| |_  ___| | |  
         | |\/| | || | \__ \ ' \/ -_) | |  
         |_|  |_|\_, | |___/_||_\___|_|_|  
                 |__/                      

╭─manu@manu:~/
╰─$ echo hello > output.txt
```


Tambien se le puede dar un archivo de entrada como parametro.
```
manu@manu:~$ make myshell
manu@manu:~$ ./myshell input/input.txt

          __  __        ___ _        _ _   
         |  \/  |_  _  / __| |_  ___| | |  
         | |\/| | || | \__ \ ' \/ -_) | |  
         |_|  |_|\_, | |___/_||_\___|_|_|  
                 |__/                      

╭─manu@manu:~/
╰─$ ls -l
drwxrwxr-x 2 manu manu  4096 Nov 17 15:37  input
-rw-rw-r-- 1 manu manu   888 Nov  8 08:51  Makefile
-rwxrwxr-x 1 manu manu 22360 Nov 17 16:08  myshell
```


## Comandos
- `cd \<directorio\>` Cambia el directorio actual a \<directorio\>.
- `clr` Limpia la pantalla
- `echo \<comentario\|env var\>` Muestra `\<comentario\>` en la pantalla o el valor de la variable de entorno `\<env var\>`.
- `quit` Cierra myshell.
- Se aceptan comandos en background mediante `&`.
- Se aceptan comandos con pipes `|`.
- Tambien se aceptan comandos externos, los cuales se ejecutan mediante `fork` y `exec`.


## Bugs
- Los paths relativos no funcionan correctamente.
- Al ejecutar algunos comandos externos se ejecuta todo correctamente, pero no se vuelve a imprimir la linea de comandos.
- El comando jobs no imprime correctamente los procesos en background a pesar que en el vector de procesos se encuentran correctamente.
- Los comandos internos tienen prioridad sobre los externos, entonces estos no se ejecutan con pipes o en background.

# TO DO
- Solucionar el bug de paths.