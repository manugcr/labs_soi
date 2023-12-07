## Paginacion
La paginacion se desarrolla para reducir la fragmentacion interna como externa que presentan las particiones.
Consiste en dividir la memoria principal en pedazos de la misma capacidad, relativamente chica, estos pedazos se llaman marcos.
Se hace lo mismo con los procesos diviendolos en pedazos de la misma capacidad que los marcos, llamadas paginas.
Al momento de cargar el proceso en memoria se asignan las paginas a un conjunto de marcos libres no necesariamente adyacentes.
El SO mantiene una lista de paginas para cada proceso.
La paginacion es invisible para el programador.
Cuando se quiere acceder a la pagina de un proceso se busca si esta en memoria principal, si no esta ocurre un fallo de pagina y se busca en memoria secundaria.
A lo anterior se lo denomina memoria virtual, que ayuda a tener procesos mas grandes de lo que puede soportar la ram.

## Segmentacion
La segmentacion se utiliza para organizar la informacion de programa y de datos otorgando diferentes permisos a cada segmento.
El compilador o el usuario definen estos segmentos de capacidad independiente y asigna datos o programa a cada uno de ellos.
En el momento de la ejecucion se asigna cada segmento a una porcion de memoria y el SO arma una tabla de segmentos.
La segmentacion si es visible para el programador.

## Particiones de memoria
Puede haber particiones dinamicas:
    Best fit, First Fit, Next Fit.
Tambien esta el buddy system.




## Modo kernel y usuario
Existen dos tipos de modos de ejecucion.
Modo kernel: No tiene restricciones.
Modo usuario: Tiene restricciones de diferentes tipos como leer o modifcar registros de control, ejecutar instrucciones de manejo de memoria, interrupciones o acceder a algunas regiones.
El procesador pasa automaticamente de modo usuario a modo kernel y viceversa en ciertos eventos o ejecucion de intrucciones, por ejemplo en la aparicion de una interrupcion.
Hay que tener cuidado en la seguridad, por que lo que se busca es vulnerar un sistema para poder escalar privilegios y tener acceso al modo kernel.

## Hilos a nivel usuario y kernel
Los hilos poseen estado y pila propia.
Hay dos categorias para definir la existencia de los hilos:
Hilos a nivel de usuario: el kernel no conoce de ellos
    Todo el trabajo de manejo de hilos lo realiza la aplicacion o proceso.
    El kernel unicamente gestiona procesos sin saber que estos a su vez generan hilos.
    Este manejo de hilos se incluye por medio de una libreria.
    Puede correr en cualquier SO.
Hilos a nivel de kernel: el kernel si esta al tanto de la existencia de los hilos.
    El kernel mantiene infomracion y administra los procesos y sus hilos.
    La planificacion del uso del procesador se realiza por el planificador del kernel a nivel de threads.
    La aplicacion no necesita realiar administracion de los threads.

## Proceso
Unidad minima de ejecucion que debe contener al menos:
    Un programa, datos e instrucciones, estado e info de control.

## Hilos
Es la unidad de ejecucion que se crea y ejecutan dentro de un proceso.
Poseen estado, contexto y pila propia. Pero estan dentro de un proceso y comparten memoria.
Tiene ventajas de ser mas rapido la creacion e intercambio, pero tiene las desventajas de concurrencia.

## Contexto de procesos
Cada proceso tiene su propio contexto program counter, stack, bloque de control, etc...
Cuando uno quiere cambiar de proceso este contexto se tiene que guardar, cargar el contexto del nuevo proceso y recien ahi ejecutarlo.
Dentro de un mismo proceso el hilo no cambia de contexto.

## Estado de procesos
En la multiprogramacion se tiene el modelo de 5 estados de un proceso, pero se le agregan dos estados mas cuando el proceso pasa de memoria secundaria/principal.
Un proceso puede ser nuevo, estar esperando a ser admitido por primera vez, el proceso existe pero su imagen no esta completa. (NEW)
Un proceso puede estar listo para ser ejecutado. (READY)
Un proceso puede estar ejecutandose. (RUNNING)
Un proceso puede estar bloqueado esperando un evento. (BLOCKED)
Un proceso puede haber terminado y estar esperando que el SO elimine la imagen. (EXIT)
Un proceso puede estar bloqueado y suspendido. (BLOCKED/SUSPENDED)
Un proceso puede estar listo y suspendido. (READY/SUSPENDED)




## DMA
El DMA permite a los dispositivos I/O acceder a la memoria para leer o escribir informacion sin necesidad del procesador.
El procesador envia la siguiente informacion al dispositivo de DMA, el tipo de operacion a realizar, cual dispositivo debe ser usado, direc inicial y la cantidad de palabras a leer o escribir.
Despues de dar estos datos el procesador vuelve a su tarea original mientras el DMA hace el trabajo por atras, y cuanod termina la unidad de DMA interrumpe.

## TLB
Cada referencia a memoria puede generar dos accesos consecutivos a memoria, uno para obtener la entrada de la tabla de paginas y otro para obtener el dato que buscamos.
Por eso para solucionar este problema se utiliza un cache para almacenar las referencias a las paginas mautilizadas y reducir las busquedas a memoria.
Dada una direccion virtual el hw busca la pagina referenciada en el tlb y si hace un hit obtiene el numero de marco directamente, si no la busca en la tabla de paginas, luego en la de memoria y actualiza el tlb.




## Politicas de reemplazo
La politica de reemplazo decide que pagina reemplazar cuando la memoria principal esta ocupada y hay que cargar una nueva pagina.
Optimo:
    No es aplicable en la vida real, ya que conociendo la secuencia de paginas que seran utilizadas se las ordena.
LRU:
    Reemplaza la pagina en memoria que no ha sido referenciada hace mas tiempo, es costosa de implementar.
FIFO:
    Forma un buffer circular con los marcos asignados al proceso y va reemplazando las paginas en round-robin.
    Reemplaza la pagina que ha estado en memoria por mas tiempo, pero no funciona muy bien aun que sea simple de implementar.
Reloj:
    Se asocia un bit de uso a cada marco del proceso, al cargar una pagina por primera vez se pone el bit de uso en en uno.
    Cuando una pagina es referenciada se pone en uno y cuando hay que reemplazar se busca el primer marco con el bit en cero.

## Politicas de planificacion procesos
En politicas de planificacion se busca una manera eficiente de ejecutar los procesos.
FCFS:
    Primero en llegar primero en servir. Es un sistema de cola puro no preferente.
    Favorece a los procesos intensivos en el uso del CPU.
    Un proceso corto debe esperar mucho en relacion a su tiempo de ejecucion. 
Round robin / virtual:
    Este es preferente es decir que puede interrumpir los procesos en ejecucion.
    Se divide la ejecucion en bloques de tiempo iguales entre si y cada vez que se interrumpe cuando termina su tiempo el proceso es vuelto a la cola de listos.
    Cuando un proceso es muy grande puede ocurrir que demore mucho en terminar, para eso esta round robin virtual que se agrega una cola de prioridad.
    En la cual se puede asignar ciertos procesos para que tengan prioridad sobre los demas procesos.
Primero el mas corto:
    No preferente, se elige ejecutar el proceso con menos tiempo de procesamiento.
    Puede haber inanicion en procesos largos.
Primero el menor tiempo restante:
    Es la version preferente del mas corto.
Primero mayor tasa de respuesta:
    Elige el proceso con mayor tasa de respuesta.
Realimentado:
    Penaliza gradualmente a los procesos que hace mas tiempo se estan ejecutando.
    Con la llegada de nuevos procesos puede producirse inanicion de procesos antiguos.
Planificacion Justa:
    Se usa cuando las aplicaciones de los usuarios son un conjunto de procesos o threads.
    El tiempo se divide equitativamente entre grupos y equitativamente a nivel de procesos.

## Politicas de planificacion HDD
El objetivo es poder cumplir con las peticiones de disco de una manera optima
FIFO:
    Es equitativa para todos los procesos.
    Se aproxima a ser un caso aleatorio cuando hay un numero grande de procesos.
El mas cercano primero:
    Se elige un sector y a partir de este el proximo es el que signifique menor movimiento del cabezal a partir del inicial.
    Rinde un poco mejor que FIFO.
SCAN:
    En esta politica el brazo se puede mover solo en una direccion.
    Va sirviendo pedidos hasta que llega a la pista final y cambia a la direccion contraria.
    Favorece a las pistas externas.
C-SCAN:
    Es igual a SCAN nada mas que solo sirve pedidos en una direccion para solucionar el problema de desbalance.
    Al llegar al final de la pista vuelve pero sin servir pedidos.

## Planificacion disco en LINUX
En planificacion de disco de linux se busca una manera de atender las solicitudes al disco de manera eficiente.
Ascensor:
    En la planificacion del ascensor se tiene una cola unica para R/W donde la cola se mantiene ordenada continuamente.
    El cabezal solo se mueve en una direccion y se agregan las condiciones:
        Si un pedido es en el mismo sector o uno adyacente a un pedido que ya esta en cola, estos se fusionan.
        Si un pedido es viejo entonces un nuevo pedido sera agregado despues de este.
        Si hay un lugar adecuado se inserta el pedido en el lugar correcto de la cola.
        Si no hay lugar se agrega al final de la cola.
Deadline:
    Coloca los pedidos en la cola del ascensor y en una FIFO de lectura o en una de escritura con un tiempo limite.
    La escritura no bloquea el proceso, pero la de lectura si.
Anticipacion:
    Cuando toma un pedido de lectura espera un ratito pq es probable a que llegue otro pedido de lectura muy cerca.
    Si llega lo ejecuta si no termina.
Completamente justo:
    Se mantiene una cola de E/S por cada proceso y se las atiende en forma de round-robin.
    Los pedidos se ordenan.
no-op:
    Igual que el justo pero los pedidos no se ordenan.





## RAID
Es un conjunto de discos conectados al sistema que son considerados como un solo disco logico, la informacion se distribuye entre los diferentes discos fisicos.
Tiene dos objetivos, uno es guardar la informacion de manera redudante para lograr resistencia frente a fallos de discos, y el otro es mejorar la performance de velocidad por medio de paralelismo.
RAID 0: No incluye redundancia, si no que los datos se dividen en bandas y se distribuyen en diferentes discos para mejorar la velocidad.
RAID 1: Este si incluye redundancia simplemente duplicando los datos en dos discos distintos.
RAID 10: Es la suma de raid 0 y raid 1.
RAID 2: Provee redundancia utilizando un codigo hamming, tambien puede corregir errores de un disco y detectar errores de dos discos. Necesita sincronizacion de cabezales entre discos.
RAID 3: Utiliza paridad a nivel de byte en un solo disco, tolera el fallo de una unidad y la paridad se calcula con OR exclusivo.
Raid 4: Utiliza tambien el disco de paridad pero puede tener bottle neck.
RAID 5: Se distribuye esta informacion de paridad del raid 3-4 pero entre los diferentes discos, el objetivo es reducir el cuello de botella de RAID 4. Tolera el fallo de un disco.
RAID 6: Igual que el 5 pero tolera el fallo de dos discos.

## Cache de disco
Se reserva un espacio de memoria principal apra almacenar sectores del disco usados recientemente.
Hay que decidir que bloques se encuentran aca, para primero verificar si estan antes de ir a memoria.
Menos usado recientemente:
    Se utiliza una pila para reemplazar el bloque que ha estado mas tiempo sin referencias.
Menos referenciado:
    Descarto el que tuvo menos referencias.
    Pero hay un problema, puedo tener muchas referencias a un bloque al inicio y luego nunca usarlo.
    Por eso se divide en secciones, nueva, medios y viejos. El contador de referencias solo aumenta si el bloque estaba en la seccion viejos.




## Organizacion y acceso a los archivos.
Para organizar la informacion dentro de una archivo tenemos varias maneras.
Archivo secuencial:
    Siempre se respeta un fortmato fijo. Los registros son de la misma longitud.
    Un campo es el campo clave y es diferente para cada registro.
    Los registros se guardan en orden en funcion del campo clave.
    Buscar unr egistro particular indicado por su campo clave implica buscar a lo largo del archivo hasta encontrar el correcto.
Secuencial indexado:
    Mantiene la estructura del secuencial pero tiene dos agregados, un indice y un archivo de overflow.
    El indice sirve para buscar mas rapido un registro particular.
Indexado:
    Misma idea pero con indices exhaustivos.
    Ahora puedo buscar registros en base a cualquier campo.
    En vez de buscar registros busco indices lo que es mas rapido.
Pilas:
    Aca los registros no tienen un tamanio definido, por lo que es muy dificil e ineficiente buscar un registro en particular.
    Es bueno solo si quiero guardar muchos datos y sacarlos todos juntos.




## Asignacion de archivos
Contigua:
    Un bloque seguido de otro se asignan al momento de la creacion.
    Tiene mucha fragmentacion.
Encadenada:
    Cada bloque tiene la direccion del siguiente.
    Para leer el ultimo tengo que pasar por todos.
Indexada:
    Se genera un inidice con una entrada para cada porcion del archivo indicando su ubicacion.
    El indice suele ser parte del mismo archivo y la tabla de asignacion solo apunta al sector donde se encuentra el indice.

## Gestion de espacio libre
Tablas de bits:
    Un vector que contiene un bit representando cada bloque del dispositivo de almacenamiento secundario.
    Si esta en cero indica que esta libre, si esta en uno indica que esta ocupado.
    Puede llegar a ser un vector muyr largo.
Porciones libres encadenadas:
    Cada porcion tiene libre tiene la direccion de la siguiente porcion libre.
    El problema es que para escribir un bloque tengo que leer, termino realizando dos operaciones.
Indexacion:
    Considera el espacio libre como un archivo mas, misma utilizacion que indexacion en archivos.
Lista de bloques libres:
    Es una lista de los bloques que estan libres.
    Cada uno identificado con un numero.

## Buffer intermedio E/S
en operaciones de transferencia de datos por E/S las paginas de memoria donde estan los datos no pueden suspenderse por mas que el proceso este bloqueado.
Si se realiza lectura adelantada se puede acelerar la velocidad de transferencia.
La idea sería armar una estructura tic toc de buffers para que se pueda escribir uno mientras se lee el otro. 
Se puede armar una estructura de buffers circulares que se cargan de a poco y se procesan rápidamente.
Esto reduce los picos en la demanda de operaciones E/S. Ya que si la demanda es alta durante periodos sostenidos, el buffer se llena o vacia bloqueadno procesos.
Mejora la respuesta promedio del sistema en general.

## ext2
En ext2 cada archivo esta descripto por una estructura de datos llamada inodo identificada por un numero unico.
Los inodos estan todos agrupados en las tablas de inodos.
Los directorios son archivos identificados por su inodo, que contienen punteros a los inodos de los archivos pertenecientes al directorio.

En el sistema ext2 divide el disco en grupos de bloques. 
En este sistema los archivos se dividen en bloques de tamaño fijo, cada archivo tiene asociado un inodo que tiene los metadatos del archivo(propietario, permisos, tamaño y una lista de bloques), 
que están almacenados en una lista de inodos. 
En el inodo vas a encontrar la lista de bloques que almacenan los datos del archivo Luego cuando un archivo se crea los bloques le son 
asignados de manera contigua en el mismo grupo de bloques, si no quedan bloques en el mismo grupo el sistema ext2 busca bloques de otros grupos.
Los bloques pueden ser directos, indirectos o doblemente directos.





